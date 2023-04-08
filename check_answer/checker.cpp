#include "checker.hpp"
#include "language.h"
#include <typeinfo>
#include <sstream>

static constexpr double eps = 1e-6;

bool Place::operator == (const Place& other) const
{
    if (typeid(*this) != typeid(other))
        return false;
    return getId() == other.getId();
}

bool Place::operator != (const Place& other) const
{
    return !(*this == other);
}

Floor::~Floor()
{
    while (!events.empty())
    {
        delete events.front();
        events.pop();
    }
}

void Floor::addOpenEvent(int elevatorId, double time)
{
    if (eventMap.find(elevatorId) != eventMap.end())
        throw OPEN_TWICE;
    FloorEvent* event = new FloorEvent;
    event->getInOnly = false;
    event->isOpen = true;
    event->elevatorId = elevatorId;
    event->time = time;
    events.push(event);
    eventMap[elevatorId] = event;
}

void Floor::addCloseEvent(int elevatorId, double time, bool getInOnly)
{
    if (eventMap.find(elevatorId) == eventMap.end())
        throw CLOSE_TWICE;
    FloorEvent* event = new FloorEvent;
    event->getInOnly = getInOnly;
    event->isOpen = false;
    event->elevatorId = elevatorId;
    event->time = time;
    events.push(event);
    eventMap[elevatorId]->getInOnly = getInOnly;
    eventMap.erase(elevatorId);
}

void Floor::checkEvents()
{
    while (!events.empty())
    {
        FloorEvent* event = events.front();
        events.pop();
        if (event->isOpen)
        {
            concurrency++;
            if (event->getInOnly)
                concurrencyGetIn++;
        }
        else
        {
            concurrency--;
            if (event->getInOnly)
                concurrencyGetIn--;
        }
        if (concurrency > maxConcurrency)
            throw CONCURRENCY_EXCEED(floor, event->time);
        if (concurrencyGetIn > maxConcurrencyGetIn)
            throw CONCURRENCY_GETIN_EXCEED(floor, event->time);
        delete event;
    }
}

bool Elevator::checkIfGetInOnly() const
{
    for (auto& passengerId : reservedPassengerIds)
    {
        if (passengerIds.count(passengerId) == 0)
            return false;
    }
    return true;
}

void Elevator::arrive(int _floor, double time)
{
    if (abs(floor - _floor) != 1) // only one floor up or down
        throw ARRIVE_TOO_MANY_FLOORS;
    if (_floor < lowestFloor || _floor > highestFloor) // in range
        throw ARRIVE_NO_SUCH_FLOOR;
    if (time < availableTime + moveTime - eps) // available
        throw ARRIVE_TOO_HURRY;
    if (isOpen()) // still open
        throw ARRIVE_NOT_CLOSED;
    if (arriveCnt == 2)
        throw ARRIVE_TOO_MANY_TIMES;
    if (maintainState == 2)
        arriveCnt++;
    floor = _floor;
    availableTime = time;
}

void Elevator::open(int _floor, double time)
{
    if (floor != _floor) // only open at the same floor
        throw OPEN_WRONG_FLOOR;
    if (isOpen()) // only open after close
        throw OPEN_TWICE;
    if (time < availableTime - eps) // available
        throw OPEN_TOO_HURRY;
    if (maintainState != 2 && !(accessMask & (1 << (floor - 1)))) // in access mask
        throw OPEN_NO_ACCESS;
    closed = false;
    availableTime = time + openTime;
    Floor::getFloor(floor).addOpenEvent(id, time);
    for (auto& passengerId : passengerIds)
        reservedPassengerIds.insert(passengerId);
}

void Elevator::close(int _floor, double time)
{
    if (floor != _floor) // only close at the same floor
        throw CLOSE_WRONG_FLOOR;
    if (closed) // only close after open
        throw CLOSE_TWICE;
    if (time < availableTime + closeTime - eps) // available
        throw CLOSE_TOO_HURRY;
    closed = true;
    availableTime = time;
    Floor::getFloor(floor).addCloseEvent(id, time, checkIfGetInOnly());
    reservedPassengerIds.clear();
}

void Elevator::maintain()
{
    if (maintainState != 2) // only maintain after accept
        throw MAINTAIN_NOT_ACCEPTED;
    if (isOpen()) // still open
        throw MAINTAIN_NOT_CLOSED;
    if (passengerIds.size() > 0) // no passenger
        throw MAINTAIN_NOT_EMPTY;
}

void Passenger::enter(Elevator* elevator, double time)
{
    if (typeid(*place) != typeid(Floor)) // only enter at floor
        throw IN_ENTER_TWICE;
    if (place->getFloor() != elevator->getFloor()) // only enter at the same floor
        throw IN_WRONG_FLOOR;
    if (!elevator->canEnter()) // elevator is available
        throw IN_FULL_CLOSED;
    place = elevator;
    elevator->passengerIds.insert(id);
}

void Passenger::exit(Elevator* elevator, double time)
{
    if (typeid(*place) != typeid(Elevator)) // only exit at elevator
        throw OUT_EXIT_TWICE;
    if (place != elevator) // only exit at the same elevator
        throw OUT_WRONG_ELEV;
    if (!elevator->canExit()) // elevator is available
        throw OUT_ELEV_CLOSED;
    place = &Floor::getFloor(elevator->getFloor());
    elevator->passengerIds.erase(id);
    endTime = time;
}

Checker::Checker()
{
    for (int i = 1; i <= elevatorCount; i++)
        elevators[i] = new Elevator(i);
}

Checker::~Checker()
{
    for (auto& passenger : passengers)
        delete passenger.second;
    for (auto& elevator : elevators)
        delete elevator.second;
}

bool Checker::checkIfCanAccessAnyWhere() const
{
    int accessMask[11] = { 0 };
    for (auto& elevator : elevators)
        for (int i = 0; i < 11; i++)
            if (elevator.second->accessMask & (1 << i))
                accessMask[i] |= elevator.second->accessMask;
    for (int step = 0; step < 11; step++)
        for (int j = 0; j < 11; j++)
            if (accessMask[0] & (1 << j))
                accessMask[0] |= accessMask[j];
    return accessMask[0] == 0x7ff;
}

void Checker::checkEvent(const Event& event)
{
    perf.lastOperatorTime = event.time;
    if (event.time < time - eps) // time is increasing
        throw WRONG_TIME_ORDER;
    time = event.time;
    auto itE = elevators.find(event.elevatorId);
    Elevator* elevator = nullptr;
    if (itE != elevators.end()) // elevator is created
        elevator = itE->second;
    auto itP = passengers.find(event.passengerId);
    Passenger* passenger = nullptr;
    if (itP != passengers.end()) // passenger is created
        passenger = itP->second;
    switch (event.type)
    {
    case EVENT_ARRIVE:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        elevator->arrive(event.curFloor, event.time);
        perf.electricCharge += 0.4;
        break;
    case EVENT_OPEN:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        elevator->open(event.curFloor, event.time);
        perf.electricCharge += 0.1;
        break;
    case EVENT_CLOSE:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        elevator->close(event.curFloor, event.time);
        perf.electricCharge += 0.1;
        break;
    case EVENT_IN:
        if (passenger == nullptr) // passenger is not created
            throw NO_PASSENGER;
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        passenger->enter(elevator, event.time);
        break;
    case EVENT_OUT:
        if (passenger == nullptr) // passenger is not created
            throw NO_PASSENGER;
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        passenger->exit(elevator, event.time);
        break;
    case EVENT_MT_AC:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        if (elevator->maintainState != 1)
            throw MAINTAIN_BAD_STATE;
        elevator->maintainState = 2;
        break;
    case EVENT_MT_ABLE:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        if (elevator->maintainState != 2)
            throw MAINTAIN_BAD_STATE;
        elevator->maintain();
        elevators.erase(event.elevatorId);
        delete elevator;
        break;
    case EVENT_REQUEST:
        if (passenger != nullptr) // passenger is not created
            throw DUPLICATE_PASSENGER;
        passengers[event.passengerId] = new Passenger(event.passengerId, event.curFloor, event.destFloor, event.time + EVENT_FIX_TIME);
        break;
    case EVENT_NEW_ELEV:
        if (elevator != nullptr) // elevator is not created
            throw DUPLICATE_ELEVATOR;
        elevators[event.elevatorId] = new Elevator(event.elevatorId, event.curFloor, event.capacity, event.accessMask, event.speed);
        break;
    case EVENT_MAINTAIN:
        if (elevator == nullptr) // elevator is not created
            throw NO_ELEVATOR;
        if (elevator->maintainState != 0)
            throw MAINTAIN_BAD_STATE;
        elevator->maintainState = 1;
        break;
    default:
        throw UNKNOWN_ACTION;
    }
    if (!checkIfCanAccessAnyWhere())
        throw ACCESS_MASK_ERROR;
}

Checker::performance Checker::checkAnswer(EventParser& parser)
{
    Checker checker;
    while (true)
    {
        try {
            parser.parseNextEvent();
            if(!parser.isAvailable()) break;
            checker.checkEvent(parser.getCurrentEvent());
        } catch (const char *msg) {
            throw parser.getCurrentLine() + " " + FORE_RED + msg + FORE_RESET_R;
        }
    }
    for (auto& passenger : checker.passengers)
    {
        auto place = passenger.second->getPlace();
        if (typeid(*place) == typeid(Elevator))
            throw PASSENGER_TRAPPED(passenger.second->id);
        if (place->getFloor() != passenger.second->to)
            throw PASSENGER_WRONG_DIST(passenger.second->id);
        checker.perf.maxWaitTime = std::max(checker.perf.maxWaitTime, passenger.second->getWaitTime());
    }
    for (auto& elevator : checker.elevators)
    {
        if (elevator.second->isOpen())
            throw ELEVATOR_NOT_CLOSED(elevator.second->id);
        if (elevator.second->maintainState != 0)
            throw ELEVATOR_NOT_MAINTAINED(elevator.second->id);
    }
    for (int i = Place::lowestFloor; i <= Place::highestFloor; i++)
    {
        auto& floor = Floor::getFloor(i);
        floor.checkEvents();
    }
    return std::move(checker.perf);
}

#include "checker.hpp"
#include <typeinfo>
#include <sstream>

//Replace checker.cpp with This, If PowerShell Doesn't Print Chinese Correctly

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

void Elevator::arrive(int _floor, double time)
{
    if (abs(floor - _floor) != 1) // only one floor up or down
        throw "arrive: Moved More Than One Floor";
    if (_floor < lowestFloor || _floor > highestFloor) // in range
        throw "arrive: No Such Floor";
    if (time < availableTime + moveTime - eps) // available
        throw "arrive: Moved Too Hurry";
    floor = _floor;
    lastAction = ACTION_MOVE;
    availableTime = time;
}

void Elevator::open(int _floor, double time)
{
    if (floor != _floor) // only open at the same floor
        throw "opendoor: No Elev Here";
    if (lastAction == ACTION_OPEN) // only open after close
        throw "opendoor: Door Already Opened";
    if (time < availableTime - eps) // available
        throw "opendoor: Elev Moving Or Closing";
    lastAction = ACTION_OPEN;
    availableTime = time + openTime;
}

void Elevator::close(int _floor, double time)
{
    if (floor != _floor) // only close at the same floor
        throw "closedoor: No Elev Here";
    if (lastAction != ACTION_OPEN) // only close after open
        throw "closedoor: Door Already Closed";
    if (time < availableTime + closeTime - eps) // available
        throw "closedoor: Close Too Hurry";
    lastAction = ACTION_CLOSE;
    availableTime = time;
}

Passenger::~Passenger()
{
    // do not delete place if it is a elevator
    if (typeid(*place) == typeid(Floor))
        delete place;
}

void Passenger::enter(Elevator* elevator, double time)
{
    if (typeid(*place) != typeid(Floor)) // only enter at floor
        throw "in: Passenger Entering Perfectly, Twice";
    if (place->getFloor() != elevator->getFloor()) // only enter at the same floor
        throw "in: Passenger Getting Into Elev Without Elev";
    if (!elevator->canEnter()) // elevator is available
        throw "in: Elev Full Or Closed";
    delete place; // delete the floor
    place = elevator;
    elevator->passengerCount++;
}

void Passenger::exit(Elevator* elevator, double time)
{
    if (typeid(*place) != typeid(Elevator)) // only exit at elevator
        throw "out: Passenger, From An Empty Elev";
    if (place != elevator) // only exit at the same elevator
        throw "out: Passenger, In Other Elev";
    if (!elevator->canExit()) // elevator is available
        throw "out: Elev Closed";
    place = new Floor(elevator->getFloor());
    elevator->passengerCount--;
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

void Checker::checkEvent(const Event& event)
{
    if (event.time < time - eps) // time is increasing
        throw "Wrong Time Flow";
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
            throw "No Such Elev";
        elevator->arrive(event.curFloor, event.time);
        break;
    case EVENT_OPEN:
        if (elevator == nullptr) // elevator is not created
            throw "No Such Elev";
        elevator->open(event.curFloor, event.time);
        break;
    case EVENT_CLOSE:
        if (elevator == nullptr) // elevator is not created
            throw "No Such Elev";
        elevator->close(event.curFloor, event.time);
        break;
    case EVENT_IN:
        if (passenger == nullptr) // passenger is not created
            throw "No Such Passenger";
        if (elevator == nullptr) // elevator is not created
            throw "No Such Elev";
        passenger->enter(elevator, event.time);
        break;
    case EVENT_OUT:
        if (passenger == nullptr) // passenger is not created
            throw "No Such Passenger";
        if (elevator == nullptr) // elevator is not created
            throw "No Such Elev";
        passenger->exit(elevator, event.time);
        break;
    case EVENT_REQUEST:
        if (passenger != nullptr) // passenger is not created
            throw "Wrong Passenger Id";
        passengers[event.passengerId] = new Passenger(event.passengerId, event.curFloor, event.destFloor);
        break;
    default:
        throw "Sorry, I dont understand";
    }
}

#define FORE_RED    "\x1b[31m"
#define FORE_RESET  "\x1b[39m"

template<typename T>
std::string toString(T x)
{
    std::ostringstream ss;
    ss << x;
    return ss.str();
}

void Checker::checkAnswer(EventParser& parser)
{
    Checker checker;
    while (true)
    {
        try {
            parser.parseNextEvent();
            if(!parser.isAvailable()) break;
            checker.checkEvent(parser.getCurrentEvent());
        } catch (const char *msg) {
            throw parser.getCurrentLine() + " " + FORE_RED + msg + FORE_RESET;
        }
    }
    for (auto& passenger : checker.passengers)
    {
        auto place = passenger.second->getPlace();
        if (typeid(*place) == typeid(Elevator))
            throw "Passenger Left On Elev" FORE_RED " Passenger(" + toString(passenger.second->id) + ")" FORE_RESET;
        if (place->getFloor() != passenger.second->to)
            throw "Passenger Not At Destination" FORE_RED " Passenger(" + toString(passenger.second->id) + ")" FORE_RESET;
    }
    for (auto& elevator : checker.elevators)
    {
        if (elevator.second->isOpen())
            throw "Door Left Open" FORE_RED " Elev(" + toString(elevator.second->id) + ")" FORE_RESET;
    }
}

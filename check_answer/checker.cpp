#include "checker.hpp"
#include <typeinfo>
#include <sstream>

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
        throw "移动超过一层";
    if (_floor < lowestFloor || _floor > highestFloor) // in range
        throw "移动到不存在的楼层";
    if (time < availableTime + moveTime) // available
        throw "移动时间不足";
    floor = _floor;
    lastAction = ACTION_MOVE;
    availableTime = time;
}

void Elevator::open(int _floor, double time)
{
    if (floor != _floor) // only open at the same floor
        throw "电梯不在该楼层";
    if (lastAction == ACTION_OPEN) // only open after close
        throw "电梯已经开门";
    if (time < availableTime) // available
        throw "电梯仍在移动/关门";
    lastAction = ACTION_OPEN;
    availableTime = time + openTime;
}

void Elevator::close(int _floor, double time)
{
    if (floor != _floor) // only close at the same floor
        throw "电梯不在该楼层";
    if (lastAction != ACTION_OPEN) // only close after open
        throw "电梯未开门";
    if (time < availableTime + closeTime) // available
        throw "关门时间不足";
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
        throw "乘客已经在电梯中";
    if (place->getFloor() != elevator->getFloor()) // only enter at the same floor
        throw "乘客和电梯不在同一楼层";
    if (!elevator->canEnter()) // elevator is available
        throw "电梯已满或未开门";
    delete place; // delete the floor
    place = elevator;
    elevator->passengerCount++;
}

void Passenger::exit(Elevator* elevator, double time)
{
    if (typeid(*place) != typeid(Elevator)) // only exit at elevator
        throw "乘客不在电梯中";
    if (place != elevator) // only exit at the same elevator
        throw "乘客不在指定电梯中";
    if (!elevator->canExit()) // elevator is available
        throw "电梯未开门";
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
    if (event.time < time) // time is increasing
        throw "时间不是递增的";
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
            throw "电梯不存在";
        elevator->arrive(event.curFloor, event.time);
        break;
    case EVENT_OPEN:
        if (elevator == nullptr) // elevator is not created
            throw "电梯不存在";
        elevator->open(event.curFloor, event.time);
        break;
    case EVENT_CLOSE:
        if (elevator == nullptr) // elevator is not created
            throw "电梯不存在";
        elevator->close(event.curFloor, event.time);
        break;
    case EVENT_IN:
        if (passenger == nullptr) // passenger is not created
            throw "乘客不存在";
        if (elevator == nullptr) // elevator is not created
            throw "电梯不存在";
        passenger->enter(elevator, event.time);
        break;
    case EVENT_OUT:
        if (passenger == nullptr) // passenger is not created
            throw "乘客不存在";
        if (elevator == nullptr) // elevator is not created
            throw "电梯不存在";
        passenger->exit(elevator, event.time);
        break;
    case EVENT_REQUEST:
        if (passenger != nullptr) // passenger is not created
            throw "乘客 ID 重复";
        passengers[event.passengerId] = new Passenger(event.passengerId, event.curFloor, event.destFloor);
        break;
    default:
        throw "未知事件";
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
    while (parser.isAvailable())
    {
        try {
            checker.checkEvent(parser.getCurrentEvent());
            parser.parseNextEvent();
        } catch (const char *msg) {
            throw parser.getCurrentLine() + " " + FORE_RED + msg + FORE_RESET;
        }
    }
    for (auto& passenger : checker.passengers)
    {
        auto place = passenger.second->getPlace();
        if (typeid(*place) == typeid(Elevator))
            throw "电梯系统结束后，" FORE_RED "乘客(" + toString(passenger.second->id) + ")未下电梯" FORE_RESET;
        if (place->getFloor() != passenger.second->to)
            throw "电梯系统结束后，" FORE_RED "乘客(" + toString(passenger.second->id) + ")未到达目的地" FORE_RESET;
    }
    for (auto& elevator : checker.elevators)
    {
        if (elevator.second->isOpen())
            throw "电梯系统结束后，" FORE_RED "电梯(" + toString(elevator.second->id) + ")未关门" FORE_RESET;
    }
}

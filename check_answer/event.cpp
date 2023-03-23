#include "event.hpp"
#include <cstring>
#include <iomanip>
#include <cstdio>

std::ostream &operator<<(std::ostream &_oStr, const Event &event)
{
    auto width = _oStr.width(0);
    _oStr << "[" << std::setw(width) << event.time << "] ";
    switch (event.type)
    {
    case EVENT_REQUEST:
        _oStr << "Request: Passenger(" << event.passengerId << ") ";
        _oStr << event.curFloor << " -> " << event.destFloor;
        break;
    case EVENT_ARRIVE:
        _oStr << "Arrive: Elevator(" << event.elevatorId << ") ";
        _oStr << event.curFloor;
        break;
    case EVENT_OPEN:
        _oStr << "Open: Elevator(" << event.elevatorId << ") ";
        _oStr << "at " << event.curFloor;
        break;
    case EVENT_CLOSE:
        _oStr << "Close: Elevator(" << event.elevatorId << ") ";
        _oStr << "at " << event.curFloor;
        break;
    case EVENT_IN:
        _oStr << "In: Passenger(" << event.passengerId << ")" << " -> ";
        _oStr << "Elevator(" << event.elevatorId << ") ";
        _oStr << "at " << event.curFloor;
        break;
    case EVENT_OUT:
        _oStr << "Out: Passenger(" << event.passengerId << ")" << " <- ";
        _oStr << "Elevator(" << event.elevatorId << ") ";
        _oStr << "at " << event.curFloor;
        break;
    default:
        _oStr << "Unknown event!";
        break;
    }
    return _oStr;
}

const Event &BaseEventParser::getCurrentEvent()
{
    return curEvent;
}

void EventParser::InputEventParser::parseNextEvent()
{
    if (!available) return;
    std::string str;
    getline(iStr, str);
    if (str == "")
    {
        available = false;
        return;
    }
    if (sscanf(
        str.c_str(), "[%lf]%d-FROM-%d-TO-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.destFloor
    ) == 4) { curEvent.type = EVENT_REQUEST; return; }
    available = false;
}

void EventParser::OutputEventParser::parseNextEvent()
{
    if (!available) return;
    std::string str;
    getline(iStr, str);
    if (str == "")
    {
        available = false;
        return;
    }
    if (sscanf(
        str.c_str(), "[%lf]ARRIVE-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_ARRIVE; return; }
    else if (sscanf(
        str.c_str(), "[%lf]OPEN-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_OPEN; return; }
    else if (sscanf(
        str.c_str(), "[%lf]CLOSE-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_CLOSE; return; }
    else if (sscanf(
        str.c_str(), "[%lf]IN-%d-%d-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.elevatorId
    ) == 4) { curEvent.type = EVENT_IN; return; }
    else if (sscanf(
        str.c_str(), "[%lf]OUT-%d-%d-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.elevatorId
    ) == 4) { curEvent.type = EVENT_OUT; return; }
    available = false;
}

void EventParser::parseNextEvent()
{
    if (!available) return;
    const Event& e1 = inputParser.getCurrentEvent();
    const Event& e2 = outputParser.getCurrentEvent();
    if (!inputParser.isAvailable() && !outputParser.isAvailable())
    {
        available = false;
        return;
    }
    else if (!inputParser.isAvailable())
    {
        curEvent = e2;
        outputParser.parseNextEvent();
    }
    else if (!outputParser.isAvailable())
    {
        curEvent = e1;
        inputParser.parseNextEvent();
    }
    else if (e1.time < e2.time)
    {
        curEvent = e1;
        inputParser.parseNextEvent();
    }
    else
    {
        curEvent = e2;
        outputParser.parseNextEvent();
    }
}

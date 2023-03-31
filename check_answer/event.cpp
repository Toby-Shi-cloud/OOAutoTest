#include "event.hpp"
#include "language.h"
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
    case EVENT_NEW_ELEV:
        _oStr << "New Elevator: Elevator(" << event.elevatorId << ") ";
        _oStr << "at " << event.curFloor << " with capacity = ";
        _oStr << event.capacity << " and speed = " << event.speed;
        break;
    case EVENT_MAINTAIN:
        _oStr << "Maintain: Elevator(" << event.elevatorId << ")";
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
    case EVENT_MT_AC:
        _oStr << "Maintain Accept: Elevator(" << event.elevatorId << ")";
        break;
    case EVENT_MT_ABLE:
        _oStr << "Maintain Able: Elevator(" << event.elevatorId << ")";
        break;
    default:
        _oStr << "Unknown event!";
        break;
    }
    return _oStr;
}

void EventParser::InputEventParser::parseNextEvent()
{
    if (!available) return;
    curLine = "";
    getline(iStr, curLine);
    if (curLine == "")
    {
        available = false;
        return;
    }
    if (sscanf(
        curLine.c_str(), "[%lf]%d-FROM-%d-TO-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.destFloor
    ) == 4) { curEvent.type = EVENT_REQUEST; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]ADD-Elevator-%d-%d-%d-%lf",
        &curEvent.time, &curEvent.elevatorId,
        &curEvent.curFloor, &curEvent.capacity, &curEvent.speed
    ) == 5) { curEvent.type = EVENT_NEW_ELEV; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]MAINTAIN-Elevator-%d",
        &curEvent.time, &curEvent.elevatorId
    ) == 2) { curEvent.type = EVENT_MAINTAIN; return; }
    available = false;
    throw UNKNOWN_FORMAT_INPUT;
}

void EventParser::OutputEventParser::parseNextEvent()
{
    if (!available) return;
    curLine = "";
    getline(iStr, curLine);
    if (curLine == "")
    {
        available = false;
        return;
    }
    if (sscanf(
        curLine.c_str(), "[%lf]ARRIVE-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_ARRIVE; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]OPEN-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_OPEN; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]CLOSE-%d-%d",
        &curEvent.time, &curEvent.curFloor, &curEvent.elevatorId
    ) == 3) { curEvent.type = EVENT_CLOSE; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]IN-%d-%d-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.elevatorId
    ) == 4) { curEvent.type = EVENT_IN; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]OUT-%d-%d-%d",
        &curEvent.time, &curEvent.passengerId,
        &curEvent.curFloor, &curEvent.elevatorId
    ) == 4) { curEvent.type = EVENT_OUT; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]MAINTAIN_ACCEPT-%d",
        &curEvent.time, &curEvent.elevatorId
    ) == 2) { curEvent.type = EVENT_MT_AC; return; }
    else if (sscanf(
        curLine.c_str(), "[%lf]MAINTAIN_ABLE-%d",
        &curEvent.time, &curEvent.elevatorId
    ) == 2) { curEvent.type = EVENT_MT_ABLE; return; }
    available = false;
    throw UNKNOWN_FORMAT_OUTPUT;
}

void EventParser::parseNextEvent()
{
    if (!available) return;
    if (curEvent.type == EVENT_NONE)
    {
        inputParser.parseNextEvent();
        outputParser.parseNextEvent();
    }
    Event e1 = inputParser.getCurrentEvent();
    e1.time -= EVENT_FIX_TIME; // 避免出现计时误差
    const Event& e2 = outputParser.getCurrentEvent();
    if (!inputParser.isAvailable() && !outputParser.isAvailable())
    {
        available = false;
        return;
    }
    else if (!inputParser.isAvailable())
    {
        curEvent = e2;
        curLine = outputParser.getCurrentLine();
        outputParser.parseNextEvent();
    }
    else if (!outputParser.isAvailable())
    {
        curEvent = std::move(e1);
        curLine = inputParser.getCurrentLine();
        inputParser.parseNextEvent();
    }
    else if (e1.time < e2.time)
    {
        curEvent = std::move(e1);
        curLine = inputParser.getCurrentLine();
        inputParser.parseNextEvent();
    }
    else
    {
        curEvent = e2;
        curLine = outputParser.getCurrentLine();
        outputParser.parseNextEvent();
    }
}

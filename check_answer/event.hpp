#ifndef _EVENT_HPP
#define _EVENT_HPP
#include <iostream>

#define EVENT_REQUEST   0
#define EVENT_ARRIVE    1
#define EVENT_OPEN      2
#define EVENT_CLOSE     3
#define EVENT_IN        4
#define EVENT_OUT       5
struct Event
{
    int type;
    int passengerId;
    int elevatorId;
    int curFloor;
    int destFloor;
    double time;
};

std::ostream& operator << (std::ostream&, const Event&);

class BaseEventParser
{
protected:
    Event curEvent = {};
    bool available = true;
public:
    bool isAvailable() { return available; }
    const Event& getCurrentEvent();
    virtual void parseNextEvent() = 0;
};

class EventParser: public BaseEventParser
{
private:
    class InputEventParser: public BaseEventParser
    {
    private:
        std::istream& iStr;
    public:
        InputEventParser(std::istream &_iStr): iStr(_iStr)
            { parseNextEvent(); }
        void parseNextEvent() override;
    } inputParser;

    class OutputEventParser: public BaseEventParser
    {
    private:
        std::istream& iStr;
    public:
        OutputEventParser(std::istream &_iStr): iStr(_iStr)
            { parseNextEvent(); }
        void parseNextEvent() override;
    } outputParser;
public:
    EventParser(std::istream &_i1, std::istream &_i2)
        : inputParser(_i1), outputParser(_i2) { parseNextEvent(); }
    void parseNextEvent() override;
};

#endif /* _EVENT_HPP */
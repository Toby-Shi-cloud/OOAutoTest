#ifndef _EVENT_HPP
#define _EVENT_HPP
#include <iostream>

#define EVENT_NONE      0
#define EVENT_REQUEST   1
#define EVENT_ARRIVE    2
#define EVENT_OPEN      3
#define EVENT_CLOSE     4
#define EVENT_IN        5
#define EVENT_OUT       6
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
    std::string curLine;
    bool available = true;
public:
    bool isAvailable() const { return available; }
    const Event& getCurrentEvent() const { return curEvent; }
    const std::string getCurrentLine() const { return curLine; }
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
        InputEventParser(std::istream &_iStr): iStr(_iStr) {}
        void parseNextEvent() override;
    } inputParser;

    class OutputEventParser: public BaseEventParser
    {
    private:
        std::istream& iStr;
    public:
        OutputEventParser(std::istream &_iStr): iStr(_iStr) {}
        void parseNextEvent() override;
    } outputParser;
public:
    EventParser(std::istream &_i1, std::istream &_i2)
        : inputParser(_i1), outputParser(_i2) {}
    void parseNextEvent() override;
};

#endif /* _EVENT_HPP */
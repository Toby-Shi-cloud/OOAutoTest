#ifndef _EVENT_HPP
#define _EVENT_HPP
#include <iostream>

#define EVENT_NONE      0   // 无效事件
#define EVENT_ARRIVE    11  // 电梯到达某一位置
#define EVENT_OPEN      12  // 电梯开始开门
#define EVENT_CLOSE     13  // 电梯完成关门
#define EVENT_IN        14  // 乘客进入电梯
#define EVENT_OUT       15  // 乘客离开电梯
#define EVENT_MT_AC     16  // 电梯接收到日常维护请求
#define EVENT_MT_ABLE   17  // 电梯可以开始进行日常维护
#define EVENT_REQUEST   21  // 乘客请求
#define EVENT_NEW_ELEV  22  // 增加电梯请求
#define EVENT_MAINTAIN  23  // 日常维护请求
struct Event
{
    int type;
    int passengerId;
    int elevatorId;
    int curFloor;
    int destFloor;
    int capacity;
    int accessMask;
    double speed;
    double time;
};
#define EVENT_FIX_TIME 0.5

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
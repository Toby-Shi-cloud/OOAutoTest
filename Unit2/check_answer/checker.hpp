#ifndef _CHECKER_HPP
#define _CHECKER_HPP
#include "event.hpp"
#include <queue>
#include <unordered_set>
#include <unordered_map>

class Place
{
public:
    static const int lowestFloor = 1;
    static const int highestFloor = 11;
    virtual ~Place() {}
    bool operator == (const Place&) const;
    bool operator != (const Place&) const;
    virtual int getFloor() const = 0;
    virtual int getId() const = 0;
};

class Floor: public Place
{
private:
    const int floor;
    Floor(int _floor): floor(_floor) {}
    ~Floor();
    struct FloorEvent
    {
        bool getInOnly;
        bool isOpen;
        int elevatorId;
        double time;
    };
    typedef std::queue<FloorEvent*> EventQueue;
    typedef std::unordered_map<int, FloorEvent*> EventMap;
    EventQueue events;
    EventMap eventMap;
    int concurrency = 0;
    int concurrencyGetIn = 0;
public:
    static Floor &getFloor(int _floor)
    {
        static Floor floors[11] = {
            Floor(1), Floor(2), Floor(3), Floor(4), Floor(5), Floor(6),
            Floor(7), Floor(8), Floor(9), Floor(10), Floor(11)
        };
        return floors[_floor - 1];
    }
    const int maxConcurrency = 4;
    const int maxConcurrencyGetIn = 2;
    int getFloor() const override { return floor; }
    int getId() const override { return floor; }
    void addOpenEvent(int elevatorId, double time);
    void addCloseEvent(int elevatorId, double time, bool getInOnly);
    void checkEvents(); // check if all events are legal. throw std::string if not
};

class Elevator;
class Passenger;
typedef std::unordered_set<int> IdSet;
typedef std::unordered_map<int, Elevator*> ElevatorMap;
typedef std::unordered_map<int, Passenger*> PassengerMap;

class Elevator: public Place
{
private:
    int floor;
    int arriveCnt = 0; // arrive count after maintain
    bool closed = true;
    double availableTime = 0; // time when the elevator is available for next action
    IdSet reservedPassengerIds;
    bool checkIfGetInOnly() const;
public:
    static constexpr double openTime = 0.2;
    static constexpr double closeTime = 0.2;
    static constexpr double basicMoveTime = 0.6;

    const int id;
    const int capacity;
    const int accessMask;
    const double moveTime;
    int maintainState = 0; // 0: not in maintain, 1: request maintain, 2: accept maintain
    IdSet passengerIds;
    Elevator(int _id): floor(1), id(_id), capacity(6), accessMask(0x7FF) ,moveTime(0.4) {}
    Elevator(int _id, int _floor, int _capacity, int _accessMask, double _moveTime):
        floor(_floor), id(_id), capacity(_capacity), accessMask(_accessMask), moveTime(_moveTime) {}
    int getFloor() const override { return floor; }
    int getId() const override { return id; }
    void arrive(int _floor, double time); // move to a floor, throw if failed
    void open(int _floor, double time); // open the door, throw if failed
    void close(int _floor, double time); // close the door, throw if failed
    void maintain(); // request maintain, throw if failed
    bool isOpen() const { return !closed; }
    bool canEnter() const { return passengerIds.size() < capacity && isOpen(); }
    bool canExit() const { return passengerIds.size() > 0 && isOpen(); }
};

class Passenger
{
private:
    const Place* place;
    double startTime = 0;
    double endTime = 0;
    double exceptTime = 0;
public:
    const int id;
    const int from;
    const int to;
    Passenger(int _id, int _from, int _to, double _time): place(&Floor::getFloor(_from)), id(_id), from(_from), to(_to)
    {
        endTime = startTime = _time;
        for (int f = Elevator::lowestFloor; f <= Elevator::highestFloor; f++)
            exceptTime += abs(f - from) * Elevator::basicMoveTime;
        exceptTime /= Elevator::highestFloor - Elevator::lowestFloor + 1;
        exceptTime += Elevator::openTime + Elevator::closeTime;
        exceptTime += abs(from - to) * Elevator::basicMoveTime;
    }
    void enter(Elevator* elevator, double time); // enter the elevator, throw if failed
    void exit(Elevator* elevator, double time); // exit the elevator, throw if failed
    const Place* getPlace() const { return place; }
    double getWaitTime() const { return endTime - startTime - exceptTime; }
};

class Checker
{
private:
    double time = -1.0; // current time
    ElevatorMap elevators;
    PassengerMap passengers;
    bool checkIfCanAccessAnyWhere() const;
public:
    static const int elevatorCount = 6;

    struct performance
    {
        double lastOperatorTime = 0;
        double maxWaitTime = -1000;
        double electricCharge = 0;
    } perf;

    Checker();
    ~Checker();

    void checkEvent(const Event& event); // check an event, throw if failed
    static performance checkAnswer(EventParser& parser); // check the answer, throw if failed
};

#endif /* _CHECKER_HPP */
#ifndef _CHECKER_HPP
#define _CHECKER_HPP
#include "event.hpp"
#include <unordered_map>

class Place
{
public:
    virtual ~Place() {}
    bool operator == (const Place&) const;
    bool operator != (const Place&) const;
    virtual int getFloor() const = 0;
    virtual int getId() const = 0;
};

class Floor: public Place
{
public:
    const int floor;
    Floor(int _floor): floor(_floor) {}
    int getFloor() const override { return floor; }
    int getId() const override { return floor; }
};

class Elevator;
class Passenger;
typedef std::unordered_map<int, Elevator*> ElevatorMap;
typedef std::unordered_map<int, Passenger*> PassengerMap;

class Elevator: public Place
{
private:
    int floor;
    int arriveCnt = 0; // arrive count after maintain
    bool closed = true;
    double availableTime = 0; // time when the elevator is available for next action
public:
    static constexpr double openTime = 0.2;
    static constexpr double closeTime = 0.2;
    static constexpr double basicMoveTime = 0.6;
    static const int lowestFloor = 1;
    static const int highestFloor = 11;

    const int id;
    const int capacity;
    const double moveTime;
    int maintainState = 0; // 0: not in maintain, 1: request maintain, 2: accept maintain
    int passengerCount = 0;
    Elevator(int _id): floor(1), id(_id), capacity(6), moveTime(0.4) {}
    Elevator(int _id, int _floor, int _capacity, double _moveTime):
        floor(_floor), id(_id), capacity(_capacity), moveTime(_moveTime) {}
    int getFloor() const override { return floor; }
    int getId() const override { return id; }
    void arrive(int _floor, double time); // move to a floor, throw if failed
    void open(int _floor, double time); // open the door, throw if failed
    void close(int _floor, double time); // close the door, throw if failed
    void maintain(); // request maintain, throw if failed
    bool isOpen() const { return !closed; }
    bool canEnter() const { return passengerCount < capacity && isOpen(); }
    bool canExit() const { return passengerCount > 0 && isOpen(); }
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
    Passenger(int _id, int _from, int _to, double _time): place(new Floor(_from)), id(_id), from(_from), to(_to)
    {
        endTime = startTime = _time;
        for (int f = Elevator::lowestFloor; f <= Elevator::highestFloor; f++)
            exceptTime += abs(f - from) * Elevator::basicMoveTime;
        exceptTime /= Elevator::highestFloor - Elevator::lowestFloor + 1;
        exceptTime += Elevator::openTime + Elevator::closeTime;
        exceptTime += abs(from - to) * Elevator::basicMoveTime;
    }
    ~Passenger();
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
public:
    static const int elevatorCount = 6;

    struct performance
    {
        double lastOperatorTime = 0;
        double maxWaitTime = 0;
        double electricCharge = 0;
    } perf;

    Checker();
    ~Checker();

    void checkEvent(const Event& event); // check an event, throw if failed
    static performance checkAnswer(EventParser& parser); // check the answer, throw if failed
};

#endif /* _CHECKER_HPP */
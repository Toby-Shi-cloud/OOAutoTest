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
    int floor = 1;
    bool closed = true;
    double availableTime = 0; // time when the elevator is available for next action
public:
    static const int capacity = 6;
    static constexpr double openTime = 0.2;
    static constexpr double closeTime = 0.2;
    static constexpr double moveTime = 0.4;
    static const int lowestFloor = 1;
    static const int highestFloor = 11;

    const int id;
    int passengerCount = 0;
    Elevator(int _id): id(_id) {}
    int getFloor() const override { return floor; }
    int getId() const override { return id; }
    void arrive(int _floor, double time); // move to a floor, throw if failed
    void open(int _floor, double time); // open the door, throw if failed
    void close(int _floor, double time); // close the door, throw if failed
    bool isOpen() const { return !closed; }
    bool canEnter() const { return passengerCount < capacity && isOpen(); }
    bool canExit() const { return passengerCount > 0 && isOpen(); }
};

class Passenger
{
private:
    const Place* place;
public:
    const int id;
    const int from;
    const int to;
    Passenger(int _id, int _from, int _to):
        place(new Floor(_from)), id(_id), from(_from), to(_to) {}
    ~Passenger();
    void enter(Elevator* elevator, double time); // enter the elevator, throw if failed
    void exit(Elevator* elevator, double time); // exit the elevator, throw if failed
    const Place* getPlace() const { return place; }
};

class Checker
{
private:
    double time = 0; // current time
    ElevatorMap elevators;
    PassengerMap passengers;
public:
    static const int elevatorCount = 6;

    Checker();
    ~Checker();

    void checkEvent(const Event& event); // check an event, throw if failed
    static void checkAnswer(EventParser& parser); // check the answer, throw if failed
};

#endif /* _CHECKER_HPP */
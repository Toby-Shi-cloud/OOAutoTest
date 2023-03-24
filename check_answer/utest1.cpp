#include "event.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
using namespace std;

const string inputString = "\
[4.1]1-FROM-2-TO-9\n\
[7.5]2-FROM-1-TO-10\n\
";

const string outputString = "\
[ 5.0700]ARRIVE-2-1\n\
[ 5.0710]OPEN-2-1\n\
[ 5.2880]IN-1-2-1\n\
[ 5.4860]CLOSE-2-1\n\
[ 5.9010]ARRIVE-3-1\n\
[ 6.3060]ARRIVE-4-1\n\
[ 6.7190]ARRIVE-5-1\n\
[ 7.1540]ARRIVE-6-1\n\
[ 7.5670]ARRIVE-7-1\n\
[ 7.9810]ARRIVE-8-1\n\
[ 8.0500]OPEN-1-2\n\
[ 8.2580]IN-2-1-2\n\
[ 8.3950]ARRIVE-9-1\n\
[ 8.4000]OPEN-9-1\n\
[ 8.4010]OUT-1-9-1\n\
[ 8.4650]CLOSE-1-2\n\
[ 8.8080]CLOSE-9-1\n\
[ 8.8860]ARRIVE-2-2\n\
[ 9.3020]ARRIVE-3-2\n\
[ 9.7120]ARRIVE-4-2\n\
[ 10.1290]ARRIVE-5-2\n\
[ 10.5370]ARRIVE-6-2\n\
[ 10.9490]ARRIVE-7-2\n\
[ 11.3530]ARRIVE-8-2\n\
[ 11.7610]ARRIVE-9-2\n\
[ 12.1690]ARRIVE-10-2\n\
[ 12.1690]OPEN-10-2\n\
[ 12.1720]OUT-2-10-2\n\
[ 12.5950]CLOSE-10-2\n\
";

const string correctString = "\
[ 3.6000] Request: Passenger(1) 2 -> 9\n\
[ 5.0700] Arrive: Elevator(1) 2\n\
[ 5.0710] Open: Elevator(1) at 2\n\
[ 5.2880] In: Passenger(1) -> Elevator(1) at 2\n\
[ 5.4860] Close: Elevator(1) at 2\n\
[ 5.9010] Arrive: Elevator(1) 3\n\
[ 6.3060] Arrive: Elevator(1) 4\n\
[ 6.7190] Arrive: Elevator(1) 5\n\
[ 7.0000] Request: Passenger(2) 1 -> 10\n\
[ 7.1540] Arrive: Elevator(1) 6\n\
[ 7.5670] Arrive: Elevator(1) 7\n\
[ 7.9810] Arrive: Elevator(1) 8\n\
[ 8.0500] Open: Elevator(2) at 1\n\
[ 8.2580] In: Passenger(2) -> Elevator(2) at 1\n\
[ 8.3950] Arrive: Elevator(1) 9\n\
[ 8.4000] Open: Elevator(1) at 9\n\
[ 8.4010] Out: Passenger(1) <- Elevator(1) at 9\n\
[ 8.4650] Close: Elevator(2) at 1\n\
[ 8.8080] Close: Elevator(1) at 9\n\
[ 8.8860] Arrive: Elevator(2) 2\n\
[ 9.3020] Arrive: Elevator(2) 3\n\
[ 9.7120] Arrive: Elevator(2) 4\n\
[10.1290] Arrive: Elevator(2) 5\n\
[10.5370] Arrive: Elevator(2) 6\n\
[10.9490] Arrive: Elevator(2) 7\n\
[11.3530] Arrive: Elevator(2) 8\n\
[11.7610] Arrive: Elevator(2) 9\n\
[12.1690] Arrive: Elevator(2) 10\n\
[12.1690] Open: Elevator(2) at 10\n\
[12.1720] Out: Passenger(2) <- Elevator(2) at 10\n\
[12.5950] Close: Elevator(2) at 10\n\
";

int main()
{
    istringstream iss(inputString);
    istringstream oss(outputString);
    ostringstream sout;
    EventParser ep(iss, oss);
    ep.parseNextEvent();
    sout << fixed << setprecision(4);
    while (ep.isAvailable())
    {
        sout << setw(7) << ep.getCurrentEvent() << endl;
        ep.parseNextEvent();
    }
    // cout << sout.str() << endl;
    assert(sout.str() == correctString);
    cout << "Utest1 pass!" << endl;
    return 0;
}

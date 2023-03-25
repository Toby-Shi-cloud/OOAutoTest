#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

const int dataCount = 5;
const string dic("testdata/");

int main()
{
    for (int i = 1; i <= dataCount; i++)
    {
        cout << "Testcase #" << i << ": ";
        ifstream ifs(dic + to_string(i) + ".in");
        ifstream ofs(dic + to_string(i) + ".out");
        EventParser ep(ifs, ofs);
        try {
            Checker::checkAnswer(ep);
            cout << "Accepted" << endl;
        } catch (string& msg) {
            cout << msg << endl;
        }
        ifs.close();
        ofs.close();
    }
    return 0;
}

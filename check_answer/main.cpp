#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream ifs("stdin.txt");
    ifstream ofs("stdout.txt");
    EventParser ep(ifs, ofs);
    try {
        Checker::checkAnswer(ep);
        cout << "Accepted" << endl;
    } catch (string& msg) {
        cout << msg << endl;
    }
    ifs.close();
    ofs.close();
    return 0;
}
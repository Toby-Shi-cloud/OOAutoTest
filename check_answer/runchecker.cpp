#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
    ifstream ifs("stdin.txt");
    ifstream ofs("output.txt");

    EventParser ep(ifs, ofs);
    Checker::performance perf;
    try
    {
        perf = Checker::checkAnswer(ep);
        cout << "Accepted" << endl;
    }
    catch (string &msg)
    {
        cout << msg << endl;
        exit(1);
    }
    ifs.close();
    ofs.close();
    return 0;
}

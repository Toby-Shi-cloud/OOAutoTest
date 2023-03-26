#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

const int dataCount = 5;
const string dic("testdata/");

int main(int argc, char* argv[])
{
    int k = atoi(argv[1]);
    FILE* file = fopen("log.txt", "a+");
    fprintf(file, "#Testcase %d:\n", k);
    ifstream ifs("stdin.txt");
    ifstream ofs("output.txt");
    EventParser ep(ifs, ofs);
    try {
        Checker::checkAnswer(ep);
        cout << "Accepted" << endl;
        fprintf(file, "Accepted\n");
    } catch (string& msg) {
        cout << msg << endl;
        fprintf(file, "%s\n", msg.c_str());
        fclose(file);
        return 1;
    }
    fclose(file);
    ifs.close();
    ofs.close();

    return 0;
}

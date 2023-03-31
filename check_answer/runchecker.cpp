#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char* argv[]) {
    int mode = atoi(argv[1]);
    FILE* file;
    if (mode == 1) {
        int k = atoi(argv[2]);
        file = fopen("log.txt", "a+");
        fprintf(file, "#Testcase %d:\n", k);
    }

    ifstream ifs("stdin.txt");
    ifstream ofs("output.txt");
    if (mode == 0) {
        if (argc == 3) {
            ifs = ifstream(argv[1]);
            ofs = ifstream(argv[2]);
        } else if (argc == 4) {
            ifs = ifstream(argv[2]);
            ofs = ifstream(argv[3]);
        }
    }

    EventParser ep(ifs, ofs);
    Checker::performance perf;
    try
    {
        perf = Checker::checkAnswer(ep);
        cout << "Accepted" << endl;
        fprintf(file, "Accepted\n");
    }
    catch (string &msg)
    {
        cout << msg << endl;
        fprintf(file, "%s\n", msg.c_str());
        fclose(file);
        exit(1);
    }
    fclose(file);
    ifs.close();
    ofs.close();
    return 0;
}

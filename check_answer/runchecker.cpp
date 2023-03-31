#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

void usage(const char *name)
{
    cout << "Usage: " << name << " [input_file output_file [performance_file]]" << endl;
    exit(2);
}

const char *fin = "stdin.txt";
const char *fout = "stdout.txt";
const char *fperf = nullptr;

int main(int argc, char *argv[])
{
    if (argc == 3) {
        fin = argv[1];
        fout = argv[2];
    } else if (argc == 4) {
        fin = argv[1];
        fout = argv[2];
        fperf = argv[3];
    } else if (argc != 1) {
        usage(argv[0]);
    }
    ifstream ifs(fin);
    ifstream ofs(fout);
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
    if (fperf != nullptr)
    {
        ofstream pfs(fperf);
        pfs << perf.lastOperatorTime << endl;
        pfs << perf.maxWaitTime << endl;
        pfs << perf.electricCharge << endl;
        pfs.close();
    }
    ifs.close();
    ofs.close();
    return 0;
}

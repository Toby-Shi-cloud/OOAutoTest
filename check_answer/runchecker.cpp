#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

void usage(const char *name)
{
    cout << "Usage: " << name << " [input_file] [output_file]" << endl;
    exit(127);
}

const char *fin = "stdin.txt";
const char *fout = "stdout.txt";

int main(int argc, char *argv[])
{
    if (argc == 3) {
        fin = argv[1];
        fout = argv[2];
    } else if (argc != 1) {
        usage(argv[0]);
        return 1;
    }
    ifstream ifs(fin);
    ifstream ofs(fout);
    EventParser ep(ifs, ofs);
    try
    {
        Checker::checkAnswer(ep);
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

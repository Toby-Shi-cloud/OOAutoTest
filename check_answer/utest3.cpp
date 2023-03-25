#include "data.h"
#include <iostream>
#include <fstream>
using namespace std;

const int dataCount = 5;
const string dic("testdata/");

int main()
{
    for (int i = 1; i <= dataCount; i++)
    {
        cout << "Gen Testcase #" << i << ": ";
        ofstream ifs(dic + to_string(i) + ".in", ios::out);
        ::data d;
        d.generator();
        cout << d.getData().size() << "bytes" << endl;
        d.getData(ifs);
        ifs.close();
    }
    return 0;
}
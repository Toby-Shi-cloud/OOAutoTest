#include <iostream>
#include <cstdio>
#include <io.h>
#include <direct.h>
#include "data.h"

int main(int argc, char* argv[]) {
    data new_data = data();
    new_data.generator();
    int i = atoi(argv[1]);
    char folder[10] = ".\\data";
    if (_access(folder, 0) == -1) {
        if (_mkdir(folder)) 
            return 1;
    }
    char filename[50];
    sprintf(filename, ".\\data\\data%d.txt", i);
    FILE* outfile = fopen("stdin.txt", "w+");
    fprintf(outfile, "%s", new_data.getData().c_str());
    fclose(outfile);
    
    outfile = fopen(filename, "w+");
    fprintf(outfile, "%s", new_data.getData().c_str());
    fclose(outfile);

    return 0;
}

#include <iostream>
#include <cstdio>
#include "data.h"

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#define _access access
#define _mkdir(f) mkdir(f, 0755)
#endif

int main(int argc, char* argv[]) {
    data new_data = data();
    new_data.generator();
    int i = atoi(argv[1]);
#ifdef _WIN32
    char folder[10] = ".\\input";
#else
    char folder[10] = "./input";
#endif
    if (_access(folder, 0) == -1) {
        if (_mkdir(folder)) 
            return 1;
    }
    char filename[50];
#ifdef _WIN32
    sprintf(filename, ".\\input\\data%d.txt", i);
#else
    sprintf(filename, "./input/data%d.txt", i);
#endif
    FILE* outfile = fopen("stdin.txt", "w+");
    fprintf(outfile, "%s", new_data.getData().c_str());
    fclose(outfile);
    
    outfile = fopen(filename, "w+");
    fprintf(outfile, "%s", new_data.getData().c_str());
    fclose(outfile);

    return 0;
}

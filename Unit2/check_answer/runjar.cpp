#include <cstdio>
#include <cstring>
#include "checker.hpp"
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#else
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#define Sleep(x) usleep(x)
#define _access access
#define _mkdir(f) mkdir(f, 0755)
#endif

void runchecker(const char* input, const char* output);

int main(int argc, char* argv[]) {
    char jar_name[32];
    char input[32];
    char output[32];
    char command[128];
    if (argc == 4) {
        strcpy(jar_name, argv[1]);
        strcpy(input, argv[2]);
        strcpy(output, argv[3]);
    } else {
        sprintf(jar_name, "code.jar");
        sprintf(input, "stdin.txt");
        sprintf(output, "output.txt");
    }
    sprintf(command, "readata.exe %s | java -jar %s > %s", input, jar_name, output);
    system(command);
    printf("Complete running %s\n", input);
    
    runchecker(input, output);

    return 0;
}

void runchecker(const char* input, const char* output) { // argv[1]: input argv[2]: output
    FILE* file;
    file = fopen("log.txt", "a+");

    std::ifstream ifs(input);
    std::ifstream ofs(output);
    EventParser ep(ifs, ofs);
    Checker::performance perf;
    try {
        perf = Checker::checkAnswer(ep);
        // std::cout << "Accepted" << std::endl;
        fprintf(file, "Test %s:\nAccepted\n", input);
    }
    catch (std::string &msg) {
        // std::cout << msg << std::endl;
        fprintf(file, "Test %s:\n%s\n", input, msg.c_str());
        fclose(file);
        ifs.close();
        ofs.close();
        exit(-1);
    }
    fclose(file);
    ifs.close();
    ofs.close();
}


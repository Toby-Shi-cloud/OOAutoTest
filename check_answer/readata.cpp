#include <stdio.h>
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

const double eps = 1e-6;

int main(int argc, char* argv[]) {
    double time = 0;
    char file_name[50];
    if (argc == 2) {
        strcpy(file_name, argv[1]);
    } else {
        sprintf(file_name, "stdin.txt");
    }
    FILE* file = fopen(file_name, "r");

    char buf[100];
    double tem;
    while (fgets(buf, 100, file) != NULL) {
        char command[50];
        sscanf(buf, "[%lf]%s", &tem, command);
        if (tem < time) {
            fprintf(stderr, "input error: time must increase\n");
            continue;
        }
        if (tem - time < eps) {
            printf("%s\n", command);
            continue;
        }
        Sleep((tem - time) * 1000);
        printf("%s\n", command);
        time = tem;
    }

}
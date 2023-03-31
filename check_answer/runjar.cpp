#include <cstdio>
#include <cstring>
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

int main(int argc, char* argv[]) {
    char jar_name[32];
    char datainput[32];
    char output[32];
    char command[128];
    if (argc == 4) {
        strcpy(jar_name, argv[1]);
        strcpy(datainput, argv[2]);
        strcpy(output, argv[3]);
    } else {
        sprintf(jar_name, "code.jar");
        sprintf(datainput, "stdin.txt");
        sprintf(output, "output.txt");
    }
    sprintf(command, "datainput.exe %s | java -jar %s > %s", datainput, jar_name, output);
    system(command);
    printf("Complete running %s\n", datainput);
    return 0;
}

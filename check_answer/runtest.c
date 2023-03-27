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

int main(int argc, char* argv[]) {
    int num = 1;
#ifdef _WIN32
    char folder[10] = ".\\ans";
#else
    char folder[10] = "./ans";
#endif
    if (_access(folder, 0) == -1) {
        if (_mkdir(folder)) 
            return 1;
    }
    sprintf(folder, "log.txt");
    FILE* file = fopen(folder, "w+");
    fprintf(file, "Test begin:\n");
    fclose(file);

    if (argc == 1) {
        printf("Please enter the number of testcases: ");
        scanf("%d", &num);
    }
    else {
        num = atoi(argv[1]);
    }

    for (int i = 1; i <= num; i++) {
        printf("In testcase %d:\n", i);
        char data[16] = "data";
        char jar_name[32] = ".\\code.jar";
#ifdef _WIN32
        char input[32] = ".\\datainput.exe";
        char checker[32] = ".\\checker";
#else
        char input[32] = "./datainput";
        char checker[32] = "./checker";
#endif
        char command[128];
        printf("Try to generate data%d\n", i);
#ifdef _WIN32
        sprintf(command, ".\\%s %d", data, i);
#else
        sprintf(command, "./%s %d", data, i);
#endif
        if (system(command)) {
            file = fopen(folder, "a+");
            fprintf(file, "Fail to generate data%d\n", i);
            fclose(file);
            printf("Exit program\n");
            return 0;
        }
        Sleep(300);
#ifdef _WIN32
        printf("Data%d generate complete, you can check it in input\\data%d\n", i, i);
#else
        printf("Data%d generate complete, you can check it in input/data%d\n", i, i);
#endif
        Sleep(300);
        printf("Running your program...\n");
        sprintf(command, "%s | java -jar %s > output.txt", input, jar_name);
        if (system(command)) {
            file = fopen(folder, "a+");
            fprintf(file, "Fail to run jar program\n");
            fclose(file);
            printf("Exit program\n");
            return 0;
        }
#ifdef _WIN32
        sprintf(command, "copy output.txt .\\ans\\ans%d.txt > NUL", i);
#else
        sprintf(command, "cp output.txt ./ans/ans%d.txt", i);
#endif
        if (system(command)) {
            file = fopen(folder, "a+");
            fprintf(file, "Fail to get output%d.txt\n", i);
            fclose(file);
            printf("Exit program\n");
            return 0;
        }
#ifdef _WIN32
        printf("Complete running, you can check your ans in ans\\ans%d.txt\n", i);
#else
        printf("Complete running, you can check your ans in ans/ans%d.txt\n", i);
#endif
        Sleep(300);
        printf("Testing your answer:\n");
        Sleep(300);
        sprintf(command, "%s %d", checker, i);
        system(command);
    }
    return 0;
}

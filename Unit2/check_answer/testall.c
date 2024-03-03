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
#define _rmdir rmdir
#endif

int main(int argc, char* argv[]) {
    char jar_name[20][20];
    int sum = 0;
    int testnum;
    if (argc == 1) {
        printf("Please enter the number of testcases: ");
        scanf("%d", &testnum);
    } else {
        if ((testnum = atoi(argv[1])) == 0) {
            printf("Wrong arguments!\n");
            return 1;
        }
    }
    // open input_jar
    FILE* file_input = fopen("test_jar.txt", "r");
    if (file_input == NULL) {
        printf("Failed to open test_jar.txt, please make sure you have test_jar.\n");
        return 1;
    }
    while (fgets(jar_name[sum], 20, file_input) != NULL)
        sum++;
    fclose(file_input);

    // deal with \n and \r
    for (int i = 0; i < sum; i++) {
        while (jar_name[i][strlen(jar_name[i]) - 1] == '\n' || jar_name[i][strlen(jar_name[i]) - 1] == '\r'){
            jar_name[i][strlen(jar_name[i]) - 1] = 0;
        }
    }

    char command[50];

    printf("Try to make program...");
    // make exe
#ifdef _WIN32
    sprintf(command, "mingw32-make.exe makeonly clean_o > NUL");
#else
    sprintf(command, "make makeonly clean_o");
#endif
    if (system(command)) {
        printf("%s\n", command);
        printf("Exit program\n");
        return 0;
    }
    Sleep(250);
    printf("Succes to make program.\n");

// FOR
    for (int i = 0; i < sum; i++) {
        if (strlen(jar_name[i]) == 0) {
            continue;
        }

        if (_access(jar_name[i], 0) == -1) {
            printf("Warning:\nNot found %s\n", jar_name[i]);
            Sleep(500);
            continue;
        }

        Sleep(250);
        char folder[20];
        printf("Test jar_name: %s\n", jar_name[i]);
        Sleep(500);
    #ifdef _WIN32
        sprintf(folder, ".\\%s", jar_name[i]);
    #else
        sprintf(folder, "./%s", jar_name[i]);
    #endif
        folder[strlen(folder) - 4] = 0;
        if (_access(folder, 0) == -1) {
            if (_mkdir(folder)) {
                printf("Failed to creat folder\n");
                printf("Exit program\n");
                return 1;
            }
        } else {
            #ifdef _WIN32
                sprintf(command, "rmdir /s/q %s", folder);
            #else
                sprintf(command, "rmdir -r %s", folder);
            #endif

            if (system(command)) {
                printf("Failed to remove folder\n");
                printf("Exit program\n");
                return 1;
            }
            if (_mkdir(folder)) {
                printf("Failed to creat folder\n");
                printf("Exit program\n");
                return 1;
            }
        }
        printf("Create folder: %s\n", folder);
        //printf("Create folder complete.\n");
        Sleep(500);
        printf("Copy related executable files...\n");
        // copy jar
    #ifdef _WIN32
        sprintf(command, "copy %s %s\\code.jar > NUL", jar_name[i], folder);
    #else
        sprintf(command, "cp %s %s/code.jar", jar_name[i], folder);
    #endif
        if (system(command)) {
            printf("Failed to copy %s\n", jar_name[i]);
            printf("Exit program\n");
            return 1;
        }

        // copy data.exe
    #ifdef _WIN32
        sprintf(command, "copy data.exe %s\\data.exe > NUL", folder);
    #else
        sprintf(command, "cp data %s/data", folder);
    #endif
        if (system(command)) {
            printf("Failed to copy data.exe\n");
            printf("Exit program\n");
            return 1;
        }

        // copy checker.exe
    #ifdef _WIN32
        sprintf(command, "copy checker.exe %s\\checker.exe > NUL", folder);
    #else
        sprintf(command, "cp checker %s/checker", folder);
    #endif
        if (system(command)) {
            printf("Failed to copy checker.exe\n");
            printf("Exit program\n");
            return 1;
        }

        // copy run.exe
    #ifdef _WIN32
        sprintf(command, "copy run.exe %s\\run.exe > NUL", folder);
    #else
        sprintf(command, "cp run %s/run", folder);
    #endif
        if (system(command)) {
            printf("Failed to copy run.exe\n");
            printf("Exit program\n");
            return 1;
        }

        // copy datainput.exe
    #ifdef _WIN32
        sprintf(command, "copy datainput.exe %s\\datainput.exe > NUL", folder);
    #else
        sprintf(command, "cp datainput %s/datainput", folder);
    #endif
        if (system(command)) {
            printf("Failed to copy datainput.exe\n");
            printf("Exit program\n");
            return 1;
        }

        //printf("Copy complete.\n");
        Sleep(250);
        printf("Try to start testing...\n");

    #ifdef _WIN32
        sprintf(command, "cd %s && start .\\run %d", folder, testnum);
    #else
        sprintf(command, "%s/run %d", folder, testnum);
    #endif
        Sleep(250);
        if (system(command)) {
            printf("Failed to start testing in folder %s\n", folder);
        } else {
            printf("Success to start testing, look result at %s\n", folder);
        }
        Sleep(250);
        
    }
    printf("Clean program.\n");
#ifdef _WIN32
    sprintf(command, "mingw32-make.exe clean > NUL");
#else
    sprintf(command, "make makeonly clean");
#endif

    Sleep(500);
    printf("See result in folders.");

    return 0;
}

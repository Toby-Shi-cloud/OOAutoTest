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

int main() {
    char jar_name[20][20];
    int sum = 0;
    int testnum;
    printf("Please enter the number of testcases: ");
    scanf("%d", &testnum);
    
    // open input_jar
    FILE* file_input = fopen("test_jar.txt", "r");
    while (fgets(jar_name[sum], 20, file_input) != NULL) sum++;
    fclose(file_input);

    // handle \n and \r
    for (int i = 0; i < sum; i++) {
        while (jar_name[i][strlen(jar_name[i]) - 1] == '\n' || jar_name[i][strlen(jar_name[i]) - 1] == '\r'){
            jar_name[i][strlen(jar_name[i]) - 1] = 0;
        }
    }

    char command[50];

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

// FOR
    for (int i = 0; i < sum; i++) {
        char folder[20];
        printf("jar_name: %s\n", jar_name[i]);
    #ifdef _WIN32
        sprintf(folder, "%s", jar_name[i]);
    #else
        sprintf(folder, "%s", jar_name[i]);
    #endif
        folder[strlen(folder) - 4] = 0;
        printf("folder:%s\n", folder);
        if (_access(folder, 0) == -1) {
            if (_mkdir(folder)) 
                return 1;
        }
        
        // copy jar
    #ifdef _WIN32
        sprintf(command, "copy %s .\\%s\\code.jar > NUL", jar_name[i], folder);
    #else
        sprintf(command, "cp %s ./%s/code.jar", jar_name[i], folder);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }

        // copy data.exe
    #ifdef _WIN32
        sprintf(command, "copy data.exe .\\%s\\data.exe > NUL", folder);
    #else
        sprintf(command, "cp data ./%s/data", folder);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }

        // copy checker.exe
    #ifdef _WIN32
        sprintf(command, "copy checker.exe .\\%s\\checker.exe > NUL", folder);
    #else
        sprintf(command, "cp checker ./%s/checker", folder);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }

        // copy run.exe
    #ifdef _WIN32
        sprintf(command, "copy run.exe .\\%s\\run.exe > NUL", folder);
    #else
        sprintf(command, "cp run ./%s/run", folder);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }

        // copy datainput.exe
    #ifdef _WIN32
        sprintf(command, "copy datainput.exe .\\%s\\datainput.exe > NUL", folder);
    #else
        sprintf(command, "cp datainput ./%s/datainput", folder);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }

    #ifdef _WIN32
        sprintf(command, "cd .\\%s && start .\\run %d", folder, testnum);
    #else
        sprintf(command, "./%s/run %d", folder, testnum);
    #endif
        if (system(command)) {
            printf("%s\n", command);
            printf("Exit program\n");
            return 0;
        }
        printf("finish\n");
    }
    
    printf("you can see result in folders");

    return 0;
}

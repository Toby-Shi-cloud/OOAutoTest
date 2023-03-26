#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <direct.h>

int main() {
    int num = 1;
    printf("Please enter the number of testcases: ");
    scanf("%d", &num);
    for (int i = 1; i <= num; i++) {
        printf("In testcase %d:\n", i);
        char data[16] = "data";
        char jar_name[32] = "code.jar";
        char input[32] = ".\\datainput.exe";
        char checker[32] = ".\\checker";
        char command[128];
        printf("Try to generate data%d\n", i);
        sprintf(command, ".\\%s %d", data, i);
        if (system(command)) {
            printf("Exit program\n");
            return 0;
        }
        Sleep(300);
        printf("Data%d generate complete, you can check it in data\\data%d\n", i, i);
        Sleep(300);
        printf("Running your program...\n");
        sprintf(command, "%s | java -jar %s > output.txt", input, jar_name);
        if (system(command)) {
            printf("Exit program\n");
            return 0;
        }
        
        char folder[10] = ".\\ans";
        if (_access(folder, 0) == -1) {
            if (_mkdir(folder)) 
                return 1;
        }
        sprintf(command, "copy output.txt .\\ans\\ans%i.txt >> log.txt", i);
        if (system(command)) {
            printf("Exit program\n");
            return 0;
        }

        printf("Complete running, you can check your ans in ans\\ans%d.txt\n", i);
        Sleep(300);
        printf("Testing your anser:\n");
        Sleep(300);
        system(checker);
    }
    return 0;
}

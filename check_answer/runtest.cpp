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

void rundata(const char* dataname);
int if_rundata();
void read_config(const char* key, char* value);
void __mkdir(char* filename);
void remove_space(char* src);

int main(int argc, char* argv[]) {
    char jar_name[64];
    char input[64];
    char command[64];
    char configbuf[64];
    char data_file[16] = {0};
    char out_file[16] = {0};
    int test_amount;
    int flag_rundata = if_rundata();

    if (flag_rundata) {
    #ifdef _WIN32
        sprintf(data_file, ".\\input");
        sprintf(out_file, ".\\ans");
    #else
        sprintf(data_file, "./input");
        sprintf(out_file, "./ans");
    #endif
        __mkdir(data_file);
        __mkdir(out_file);
    }

    read_config("test_amount", configbuf);
    test_amount = atoi(configbuf);
    if (test_amount == 0) {
        test_amount = 1;
    }

    read_config("jar_name", jar_name);
    if (strlen(jar_name) == 0) {
        strcpy(jar_name, "code.jar");
    }

    for (int i = 1; i <= test_amount; i++) {
        if (flag_rundata) {
        #ifdef _WIN32
            sprintf(input, ".\\input\\%d.in", i);
        #else
            sprintf(input, "./input/%d.in", i);
        #endif
            rundata(input);
        } else {
            read_config("input", input);
            if (strlen(input) == 0) {
                printf("RunTestErr: cannot read input in config\n");
                exit(-1);
            }
        }

        printf("Running your program.\n");
        
    #ifdef _WIN32
        sprintf(command, "readata.exe %s | java -jar %s > .\\ans\\%d.out", input, jar_name, i);
    #else
        sprintf(command, "start readata.exe %s | java -jar %s > ./ans/%d.out", input, jar_name, i);
    #endif
        printf(command);
        if (system(command)) {
            printf("RunTestErr: Fail to run program\n");
            exit(-1);
        } 
/*
        printf("Check your answer...\n");
        sprintf(command, "checker.exe 0 %s output.txt", input);
        if (system(command)) {
            printf("Fail to check answer\n");
            exit(-1);
        } 
        */
    }
    return 0;
}

int if_rundata() { // 1: run_data 0: no run_data
    char configbuf[50];
    read_config("readInput", configbuf);
    return !atoi(configbuf);
}

void rundata(const char* dataname) {
    printf("Generating data\n");
    char configbuf[50];
    char command[50];
    read_config("data_mode", configbuf);
    int data_mode = atoi(configbuf);

    read_config("data_amount", configbuf);
    int data_amount = atoi(configbuf);

    read_config("data_flag", configbuf);
    int data_flag = atoi(configbuf);

    read_config("data_step_size", configbuf);
    double data_step_size = atof(configbuf);
    sprintf(command, "rundata.exe %d %d %d %lf > %s", data_mode, data_amount, data_flag, data_step_size, dataname);

    system(command);
}

void read_config(const char* key, char* value) {
    FILE* config = fopen("config.txt", "r");
    if (config == NULL) {
        printf("RunTestErr: Fail to open config.txt\n");
        exit(-1);
    }
    char buf[50] = {0};
    while (fgets(buf, 50, config) != NULL) {
        if (strncmp(buf, key, strlen(key)) == 0) {
            sprintf(value, "%s", buf + strlen(key) + 1);
            remove_space(value);
            fclose(config);
            return;
        }
    }
    sprintf(value, "");
    fclose(config);
}

void __mkdir(char* filename) {
    if (_access(filename, 0) == -1) {
        if (_mkdir(filename)) 
            fprintf(stderr, "RunTestErr: Fail to mkdir\n");
            exit(-1);
    }
}

void remove_space(char* src) {
    int len = strlen(src);
    while ((src[len - 1] == '\n' || src[len - 1] == '\r') && len > 0) {
        src[len - 1] = 0;
        len--;
    }
}
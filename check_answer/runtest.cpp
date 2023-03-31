#include <stdio.h>
#include "Data.h"
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
void generatedata(const char* filename, int mode = -1, int amount = 0, int flag = -1, double step = 0);

int main(int argc, char* argv[]) {
    char jar_name[64];
    char input[64];
    char command[256];
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

    #ifdef _WIN32
        sprintf(command, "copy %s stdin.txt > NUL", input);
    #else
        sprintf(command, "cp %s stdin.txt", input);
    #endif

        printf("Running your program.\n");
        
        
    #ifdef _WIN32
        sprintf(out_file, ".\\ans\\%d.out", i);
        sprintf(command, "start /b runjar.exe %s %s %s", jar_name, input, out_file);
    #else
        sprintf(out_file, "./ans/%d.out", i);
        sprintf(command, "start runjar %s %s %s", input, jar_name, out_file);
    #endif
        // printf(command);
        if (system(command)) {
            printf("RunTestErr: Fail to run program\n");
            exit(-1);
        } 

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
    generatedata(dataname, data_mode, data_amount, data_flag, data_step_size);
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
    value[0] = 0;
    fclose(config);
}

void __mkdir(char* filename) {
    if (_access(filename, 0) == -1) {
        if (_mkdir(filename)) {
            fprintf(stderr, "RunTestErr: Fail to mkdir\n");
            exit(-1);
        }
    }
}

void remove_space(char* src) {
    int len = strlen(src);
    while ((src[len - 1] == '\n' || src[len - 1] == '\r') && len > 0) {
        src[len - 1] = 0;
        len--;
    }
}

void generatedata(const char* filename, int mode, int amount, int flag, double step) { // 参数：模式、数据数量、时间步进概率、时间步进长度
    FILE* datafile = fopen(filename, "w+");
    if (datafile == NULL) {
        fprintf(stderr, "GenerateDataErr: fail to open datafile\n");
    }
    Data new_data(mode, amount, flag, step);
    new_data.generate();
    fprintf(datafile, "%s", new_data.getData().c_str());
    fclose(datafile);

}

#include <cstdio>
#include "Data.h"

int main(int argc, char* argv[]) { // 参数：模式、数据数量、时间步进概率、时间步进长度
    
    int mode = -1;
    int amount = 0;
    int flag = -1;
    double step = 0;
    
    if (argc >= 2) {
        if ((mode = atoi(argv[1])) == 0) {
            fprintf(stderr, "Generate data error: wrong arguments-mode\n");
            exit(-1);
        }
    }
    if (argc >= 3) {
        amount = atoi(argv[2]);
    }
    if (argc >= 4) {
        if ((flag = atoi(argv[3])) == 0) {
            fprintf(stderr, "Generate data error: wrong arguments-flag\n");
            exit(-1);
        }
    }
    if (argc >= 5) {
        step = atof(argv[4]);
    }

    Data new_data(mode, amount, flag, step);
    new_data.generate();
    printf("%s", new_data.getData().c_str());
/*
#ifdef _WIN32
    char folder[10] = ".\\input";
#else
    char folder[10] = "./input";
#endif
    // 创建文件夹
    if (_access(folder, 0) == -1) {
        if (_mkdir(folder)) {
            fprintf(stderr, "Generate data error: fail to mkdir\n");
            exit(-1);
        }
    }
    FILE* outfile;
    char filename[50];
    if (argc == 1) {
        sprintf(filename, "stdin.txt");
    } else {
    #ifdef _WIN32
        sprintf(filename, ".\\input\\%s", argv[1]);
    #else
        sprintf(filename, "./input/%s", argc[1]);
    #endif
    }
    outfile = fopen("stdin.txt", "w+");
    if (outfile == NULL) {
        fprintf(stderr, "GenerateDataError: fail to open file\n");
        exit(-1);
    }
    fprintf(outfile, "%s", new_data.getData().c_str());
    fclose(outfile);
    */
    
    return 0;
}

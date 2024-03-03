#include <cstdio>
#include "Data.h"

int main(int argc, char* argv[]) { // 参数：模式、数据数量、时间步进概率、时间步进长度
    
    int mode = -1;
    int amount = 0;
    int flag = -1;
    double step = 0;
    
    if (argc >= 2) {
       mode = atoi(argv[1]);
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
    // printf("%d %f\n", new_data.getFlag(), new_data.geStep());
    new_data.generate();
    printf("%s", new_data.getData().c_str());
    
    return 0;
}

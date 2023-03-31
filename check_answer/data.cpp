//
// Created by DELL on 2023/3/25.
//

#include "Data.h"
#include <fstream>
#include <cstdlib>
#include <ctime>


Data::Data(int mode, int amount, int flag, double step_size) {
    srand(time(nullptr));
    if (mode != -1) {
        this->mode = mode;
    } else {
        this->mode = rand() % 6;
    }
    if (amount != 0) {
        this->amount = amount;
    } else {
        this->amount = rand() % 30 + 40;
    }
    if (flag != -1) {
        this->flag = flag;
    } else {
        this->flag = rand() % 90 + 5;
    }
    if (fabs(step_size) > 1e-6) {
        this->step_size = step_size;
    } else {
        this->step_size = 25.0 / (this->flag * this->flag * (double)(rand() % 50 + 50) / 100.0);
    }
    for (int i = 1; i < 20; i++) {
        elevator[i] = i <= 6 ? 1 : 0;
    }
}


std::string &Data::getData() {
    return this->content;
}

void Data::generate() {
    switch (mode) {
        case 0:
            generator_mode0();
            break;
        case 1:
            generator_mode1();
            break;
        case 2:
            generator_mode2();
            break;
        case 3:
            generator_mode3();
            break;
        case 4:
            generator_mode4();
            break;
        case 5:
            generator_mode5();
            break;
        default:
            generator_mode0();
            break;
    }

}

void Data::generator_mode0() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    while (request_num--) {
        int begin = rand() % 11 + 1;
        int end = rand() % 11 + 1;
        while (begin == end) {
            end = rand() % 11 + 1;
        }
        this->content += timer->getTime();
        personRequest(id, begin, end);
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max) {
            if (rand() % 100 < 10) {
                generateElevator();
            }
        }
        if (num_elevator > elevator_min) {
            if (rand() % 100 < 10) {
                generateMaintain();
            }
        }
        id++;
    }
}

void Data::generator_mode1() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    while (request_num--) {
        int begin = rand() % 10 + 1;
        int end = 11 - rand() % (11 - begin);
        this->content += timer->getTime();
        personRequest(id, begin, end);
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max) {
            if (rand() % 100 < 10) {
                generateElevator();
            }
        }
        if (num_elevator > elevator_min) {
            if (rand() % 100 < 10) {
                generateMaintain();
            }
        }
        id++;
    }
}

void Data::generator_mode2() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    while (request_num--) {
        int begin = rand() % 10 + 2;
        int end = rand() % (begin - 1) + 1;
        this->content += timer->getTime();
        personRequest(id, begin, end);
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max && rand() % 100 < 10) {
            generateElevator();
        }
        if (num_elevator > elevator_min && rand() % 100 < 10) {
            generateMaintain();
        }
        id++;
    }
}

void Data::generator_mode3() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    int end_0 = rand () % 11 + 1;
    while (request_num--) {
        int begin = rand() % 11 + 1;
        int end = rand() % 11 + 1;
        if (rand() % 100 > 30) {
            end = end_0;
        }
        while (begin == end) {
            end = rand() % 11 + 1;
        }
        this->content += timer->getTime();
        personRequest(id, begin, end);
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max && rand() % 100 < 10) {
            generateElevator();
        }
        if (num_elevator > elevator_min && rand() % 100 < 10) {
            generateMaintain();
        }
        id++;
    }
}

void Data::generator_mode4() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    while (request_num--) {
        int begin = rand() % 11 + 1;
        int step = rand() % 4 + 1;
        int direction = rand() % 100 < 50 ? 1 : -1;
        int end = begin + direction * step;
        while (end <= 0 || end == begin) {
            end += 5;
        }
        while (end > 11 || end == begin) {
            end -= 5;
        }
        this->content += timer->getTime();
        personRequest(id, begin, end);
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max && rand() % 100 < 10) {
            generateElevator();
        }
        if (num_elevator > elevator_min && rand() % 100 < 10) {
            generateMaintain();
        }
        id++;
    }
}

void Data::generator_mode5() {
    srand(time(nullptr));
    int id = 1;
    int request_num = this->amount;
    int num_elevator = 6;
    int elevator_min = rand() % 6 + 1;
    int elevator_max = rand() % 6 + 5;
    while (request_num--) {
        int begin = rand() % 4 + 1;
        int end = rand() % 4 + 8;
        this->content += timer->getTime();
        if (rand() % 100 < 50) {
            personRequest(id, begin, end);
        } else {
            personRequest(id, end, begin);
        }
        if (rand() % 100 <= flag) {
            timer->step(step_size);
        }
        if (num_elevator < elevator_max && rand() % 100 < 10) {
            generateElevator();
        }
        if (num_elevator > elevator_min && rand() % 100 < 10) {
            generateMaintain();
        }
        id++;
    }
}
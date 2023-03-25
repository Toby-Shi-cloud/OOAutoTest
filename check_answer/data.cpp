//
// Created by DELL on 2023/3/25.
//

#include "data.h"
#include <fstream>
#include <cstdlib>
#include <ctime>

data::data(int mode, bool jamMode): mode(mode), jamMode(jamMode) {}

data::data() {
    srand((long long)time(nullptr));
    mode = rand() % 6;
    jamMode = rand() & 1;
}

const std::string &data::getData() {
    return this->content;
}

std::ostream& data::getData(std::ostream &os) {
    os << this->content;
    os.flush();
    return os;
}

std::iostream& data::getData(std::iostream &ios) {
    std::streampos p = ios.tellp(); // save the position of ios
    ios << this->content;
    ios.flush();
    ios.seekg(p); // set the position of ios to the position of ios before writing
    return ios;
}

void data::generator() {
    time_generator time1(jamMode ? 2 : 0);
    //printf("Mode: %d, Jam: %s\n", mode, jamMode ? "true" : "false");
    switch (this->mode) {
        case 0:
            generator_mode0(time1);
            break;
        case 1:
            generator_mode1(time1);
            break;
        case 2:
            generator_mode2(time1);
            break;
        case 3:
            generator_mode3(time1);
            break;
        case 4:
            generator_mode4(time1);
            break;
        case 5:
            generator_mode5(time1);
            break;
        default:
            generator_mode0(time1);
            break;
    }
}

void data::generator_mode0(time_generator& timer) { // 平衡交通
    srand((long long)time(nullptr));
    int num = rand() % 10 + 5;
    if (jamMode)
        num += 45;
    while (num--) {
        timer.step();
        int begin = rand() % 11 + 1;
        int end = rand() % 11 + 1;
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        while (begin == end) {
            end = rand() % 11 + 1;
        }
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode1(time_generator &timer) { // 上行高峰
    srand((long long)time(nullptr));
    int num = rand() % 5 + 15;
    if (jamMode)
        num += 30;
    while (num--) {
        timer.step();
        int begin = rand() % 6 + 1;
        int end = rand() % 11 + begin + 1;
        while (end > 11) {
            end -= 3;
        }
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode2(time_generator &timer) { // 下行高峰
    srand((long long)time(nullptr));
    int num = rand() % 10 + 5;
    if (jamMode)
        num += 30;
    while (num--) {
        timer.step();
        int begin = rand() % 5 + 6;
        int end = begin - 1 - rand() % (begin - 1);
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode3(time_generator &timer) { // 二路交通
    srand((long long)time(nullptr));
    int des = rand() % 11 + 1;
    int num = rand() % 5 + 10;
    if (jamMode)
        num += 40;
    while (num--) {
        timer.step();
        int begin = rand() % 11 + 1;
        int end = rand() % 11 + 1;
        if (rand() % 100 > 35) {
            end = des;
        }
        while (begin == end) {
            end = rand() % 11 + 1;
        }
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }
}

void data::generator_mode4(time_generator &timer) { // 短途交通
    srand((long long)time(nullptr));
    int num = rand() % 5 + 10;
    if (jamMode)
        num += 55;
    while (num--) {
        timer.step();
        int begin = rand() % 11 + 1;
        int end = rand() % 3 + 1;
        end = begin + ((rand() & 1) ? 1 : -1) * end;
        while (begin == end || end > 11 || end < 1) {
            end = rand() % 11 + 1;
        }
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }
}

void data::generator_mode5(time_generator &timer) { // 长途交通
    srand((long long)time(nullptr));
    int num = rand() % 5 + 10;
    if (jamMode)
        num += 20;
    while (num--) {
        timer.step();
        int begin = rand() % 3;
        begin = (rand() & 1) ? 1 + begin : 11 - begin;
        int end = rand() % 5;
        end = begin > 6 ? 1 + end : 11 - end;
        people += rand() % 3 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }
}

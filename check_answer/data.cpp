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
    mode = rand() % 4;
    jamMode = rand() & 1;
}

std::string &data::getData() {
    return this->content;
}

std::shared_ptr<std::fstream> data::getData(std::string &file) {
    std::shared_ptr<std::fstream> outfile(new std::fstream);
    outfile->open(file);
    *outfile << this->content << std::endl;
    return outfile;
}

void data::generator() {
    time_generator time1(jamMode ? 2 : 0);

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
        default:
            generator_mode0(time1);
            break;
    }
}

void data::generator_mode0(time_generator& timer) {
    srand((long long)time(nullptr));
    int num = rand() % 10 + 5;
    if (jamMode)
        num += 40;
    while (num--) {
        timer.step();
        int begin = rand() % 11 + 1;
        int end = rand() % 11 + 1;
        people += rand() % 5 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        while (begin == end) {
            end = rand() % 11 + 1;
        }
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode1(time_generator &timer) {
    srand((long long)time(nullptr));
    int num = rand() % 5 + 15;
    while (num--) {
        timer.step();
        int begin = rand() % 6 + 1;
        int end = rand() % 11 + begin;
        while (end > 11) {
            end -= 3;
        }
        people += rand() % 5 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode2(time_generator &timer) {
    srand((long long)time(nullptr));
    int num = rand() % 10 + 5;
    if (jamMode)
        num += 40;
    while (num--) {
        timer.step();
        int begin = rand() % 6 + 5;
        int end = rand() % 6 + 1;
        if (end == begin)
            end--;
        people += rand() % 5 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }

}

void data::generator_mode3(time_generator &timer) {
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
        people += rand() % 5 + 1;
        content += "[" + timer.getTime() + "]" + std::to_string(people) + "-FROM-";
        content += std::to_string(begin) + "-TO-" + std::to_string(end) + "\n";
    }
}








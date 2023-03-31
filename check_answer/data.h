//
// Created by DELL on 2023/3/25.
//

#ifndef DATA_H
#define DATA_H
#include <string>
#include <memory>
#include <random>

/* mode
 * 0: 平衡交通
 * 1: 上行高峰
 * 2: 下行高峰
 * 3: 二路交通
 * 4: 短途交通
 * 5: 长途交通
 */
class Timer;


class Timer {
private:
    double second = 1;

public:
    Timer()= default;
    void step(double num) {
        second += num;
    }

    std::string getTime() {
        char str[20];
        sprintf(str, "[%.1lf]", second);
        return std::string(str);
    }

    Timer(Timer *pTimer) {

    }
};


class Data {
private:
    std::string content;
    int mode = 0;
    int flag;
    int amount;
    double step_size;
    int elevator[20] = {0};
    int remove[20] = {0};
    Timer *timer = new Timer();
public:
// 默认则会随机选择一种模式生成
    explicit Data(int mode = -1, int amount = 0, int flag = -1, double step_size = 0);
    ~Data() {
        delete timer;
    }

    void generate();
    void generator_mode0();
    void generator_mode1();
    void generator_mode2();
    void generator_mode3();
    void generator_mode4();
    void generator_mode5();

    void generateElevator() {
        int id = rand() % 13 + 7;
        int cnt = 0;
        while (elevator[id]) {
            id = rand() % 13 + 7;
            cnt++;
            if (cnt > 20) {
                return;
            }
        }
        int begin = rand() % 11 + 1;
        int capacity = rand() % 6 + 3;
        double speed = (double)(rand() % 5 + 2) / 10.0;
        elevator[id] = 1;
        elevatorRequest(id, begin, capacity, speed);
    }

    void generateMaintain() {
        int id = rand() % 19 + 1;
        int cnt = 0;
        while (elevator[id] != 1 || remove[id] != 0) {
            id = rand() % 19 + 1;
            cnt++;
            if (cnt > 20) {
                return;
            }
        }
        remove[id] = 1;
        maintainRequest(id);
    }

    void personRequest(int id, int start, int end) {
        char str[20];
        sprintf(str, "%d-FROM-%d-TO-%d\n", id, start, end);
        content += str;
    }
    void elevatorRequest(int id, int begin, int capacity, double speed) {
        char str[30];
        this->content += timer->getTime();
        sprintf(str, "ADD-Elevator-%d-%d-%d-%.1lf\n", id, begin, capacity, speed);
        content += str;
    }
    void maintainRequest(int id) {
        char str[30];
        this->content += timer->getTime();
        sprintf(str, "MAINTAIN-Elevator-%d\n", id);
        content += str;
    }

    std::string& getData();

};


#endif //DATA_H

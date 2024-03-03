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
    Timer() = default;

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
    bool access_map[20][11];
    bool visited[11];
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

    int getFlag() {
        return this->flag;
    }

    double geStep() {
        return this->step_size;
    }

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
        double speed = (double) (rand() % 5 + 2) / 10.0;
        int access = rand() % 2047 + 1;
        for (int i = 0; i < 11; ++i) {
            access_map[id][i] = access & (1 << i);
        }
        elevator[id] = 1;
        elevatorRequest(id, begin, capacity, speed, access);
    }

    void generateMaintain() {
        int id = rand() % 19 + 1;
        int cnt = 0;
        while (elevator[id] != 1 || remove[id] != 0 || !isAccessRemove(id)) {
            id = rand() % 19 + 1;
            cnt++;
            if (cnt > 20) {
                return;
            }
        }
        remove[id] = 1;
        maintainRequest(id);
    }

    bool isAccessRemove(int id) {
        //电梯根本不在，直接返回true
        if (elevator[id] != 1 || remove[id] != 0) {
            return true;
        }
        //前六部电梯有一部还在就返回true
        for (int i = 1; i <= 6; ++i) {
            if (remove[i] == 0) {
                return true;
            }
        }
        for (int i = 0; i < 11; ++i) {
            visited[i] = false;
        }
        //取出所有可用的电梯
        bool moveEle[20] = {0};
        for (int i = 7; i < 20; ++i) {
            if (elevator[i] == 1 && remove[i] == 0 && i != id) {
                moveEle[i] = true;
            }
        }
        //从第一层出发
        dfs(0 ,moveEle);

        bool flag = false;
        for (int i = 0; i < 11; ++i) {
            if (!visited[i]) {
                flag = true;
                visited[i] = true;
            }
        }
        return flag;
    }

    void dfs(int dep, bool moveEle[20]) {
        visited[dep] = true;
        for (int i = 0; i < 11; ++i) {
            if (!visited[i] && isConnected(dep, i, moveEle)) {
                dfs(i, moveEle);
            }
        }
    }

    bool isConnected(int from, int to, bool moveEle[20]) {
        for (int i = 1; i < 20; ++i) {
            if (moveEle[i]) {
                if(access_map[i][from] && access_map[i][to]) {
                    return true;
                }
            }
        }
        return false;
    }

    void personRequest(int id, int start, int end) {
        char str[20];
        sprintf(str, "%d-FROM-%d-TO-%d\n", id, start, end);
        content += str;
    }

    void elevatorRequest(int id, int begin, int capacity, double speed, int access) {
        char str[30];
        this->content += timer->getTime();
        sprintf(str, "ADD-Elevator-%d-%d-%d-%.1lf-%d\n", id, begin, capacity, speed, access);
        content += str;
    }

    void maintainRequest(int id) {
        char str[30];
        this->content += timer->getTime();
        sprintf(str, "MAINTAIN-Elevator-%d\n", id);
        content += str;
    }

    std::string &getData();

};


#endif //DATA_H

//
// Created by DELL on 2023/3/25.
//

#ifndef DATA_H
#define DATA_H
#include <string>
#include <random>

/* mode
 * 0: 平衡交通
 * 1: 上行高峰
 * 2: 下行高峰
 * 3: 二路交通
 * 4: 短途交通
 * 5: 长途交通
 * jamMode: 拥堵模式（在短时间内进行所有请求）
 */
class time_generator;
class data {
private:
    std::string content;
    int mode = 0;
    bool jamMode = false;
    bool publicTest = false;
    int people = 1;
public:
    data(); // 默认则会随机选择一种模式生成
    data(int mode, bool jamMode);
    data(int mode, bool jamMode, bool publicTest): mode(mode), jamMode(jamMode), publicTest(publicTest){}
    void generator();
    void generator_mode0(time_generator& timer);
    void generator_mode1(time_generator& timer);
    void generator_mode2(time_generator& timer);
    void generator_mode3(time_generator& timer);
    void generator_mode4(time_generator& timer);
    void generator_mode5(time_generator& timer);
    const std::string& getData();
    std::ostream& getData(std::ostream& os); // output data to os and return os
    std::iostream& getData(std::iostream& ios); // output data to ios and return ios, then you can read from this ios
};

class time_generator {
private:
    int second = 0;
    int d_second = 0;
    int min;
    int max;
    bool publicTest = false;
    std::random_device device;
    std::default_random_engine engine;
    std::uniform_int_distribution<int> distribution;
public:

    explicit time_generator(int mode) {
        switch (mode) {
            case 0:
                min = 200;
                max = 1000;
                break;
            case 1:
                min = 100;
                max = 200;
                break;
            case 2:
                min = 70;
                max = 110;
                break;
            default:
                min = 100;
                max = 1000;
        }
        if (!publicTest)
            second = 1;
        engine = std::default_random_engine (device());
        distribution = std::uniform_int_distribution<int>(min, max);
    }

    time_generator(int mode, bool publicTest) {
        this->publicTest = publicTest;
        switch (mode) {
            case 0:
                min = 200;
                max = 1000;
                break;
            case 1:
                min = 100;
                max = 200;
                break;
            case 2:
                min = 70;
                max = 110;
                break;
            default:
                min = 100;
                max = 1000;
        }
        if (!publicTest)
            second = 1;
        engine = std::default_random_engine (device());
        distribution = std::uniform_int_distribution<int>(min, max);
    }
    void step() {
        d_second += distribution(engine);
        while (d_second >= 1000) {
            d_second -= 1000;
            second++;
        }
    }

    std::string getTime() {
        char output[40];
        sprintf(output, "%3d.%03d", second, d_second / 100 * 100);
        return output;
    }
};


#endif //DATA_H

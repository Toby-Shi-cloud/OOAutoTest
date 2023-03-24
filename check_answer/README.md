# Checker

## Structure

```
.
├── Makefile
├── README.md
├── checker.cpp
├── checker.hpp
├── event.cpp
├── event.hpp
├── testdata
│   ├── 1.in
│   ├── 1.out
│   ├── 2.in
│   ├── 2.out
│   ├── 3.in
│   ├── 3.out
│   ├── 4.in
│   ├── 4.out
│   ├── 5.in
│   └── 5.out
├── utest1.cpp
└── utest2.cpp
```

## Unit Test

### Utest 1

`utest1.cpp` 用于测试 `EventParser`。编译指令: `make utest1`。

执行 `./utest1` 的结果: `Utest1 pass!`。

### Utest 2

`utest2.cpp` 用于测试 `Checker`。编译指令: `make utest2`。

执行 `./utest2` 的结果:
```
Testcase #1: Accepted
Testcase #2: [ 4.6280]OUT-1-1-1 乘客不在电梯中
Testcase #3: 电梯系统结束后，电梯(2)未关门
Testcase #4: [ 6.2259]IN-2-10-1 时间不是递增的
Testcase #5: [  1.6610]IN-1-4-1 乘客不存在
```
即说明 `Checker` 可以正常指出 `testdata` 中的错误。

## Document

### `EventParser`

使用时应先 `#include "event.hpp"`。

- `EventParser::EventParser(std::istream &_i1, std::istream &_i2)`
> 类 `EventParser` 构造方法要求传入两个 `std::istream&`，即两个输入流，第一个输入流是输入数据，第二个是输出数据。注意这两个输入流不能相同或有冲突。
>
> 由于 `std::ifstream` 和 `std::istringstream` 均从 `std::istream` 派生，所以当需要以文件作为输入时用 `std::ifstream`，需要直接使用字符串时用 `std::istringstream` (当然 `std::fstream` 和 `std::stringstream` 也是可用的)。

- `void EventParser::parseNextEvent() throw(const char*)`
> 成员方法 `parseNextEvent` 会进行一次读入，并将下一个事件设置为 `curEvent`，`curEvent` 可以通过 `getCurrentEvent()` 方法获得 (该方法返回一个不可变引用)。
> 
> 当输入流全部为空时，将 `available` 置 `0`。当输入流数据格式异常时，抛出 `const char*`。

- `struct Event`
> 用于记录事件。可以支持流式输出。

### `Checker`

使用时应先 `#include "checker.hpp"`。

- `Checker::Checker()`
> 类 `Checker` 的默认构造函数。会自动构造 `6` 个电梯。

- `Checker::~Checker()`
> 类 `Checker` 的析构函数。会析构所有包含的 `Elevator` 对象和 `Passenger` 对象。

- `static void Checker::checkAnswer(EventParser& parser) throw(std::string)`
> 静态方法 `Checker::checkAnswer` 要求传入一个 `EventParser&` 对象。用于判断该组输入输出是否合法。该方法为 `void` 类型，若比对不合法会抛出一个 `std::string` 表示异常原因，所以使用时应考虑使用 `try-catch` 结构。

- `void Checker::checkEvent(const Event& event) throw(const char*)`
> 成员方法 `Checker::checkEvent` 要求传入一个 `Event&` 对象。用于在已有 `Checker` 实例基础上，进一步判断接下来的一个事件是否合法。同样为 `void` 类型，不合法时抛出 `const char*` 表示异常原因。

## Example

样例程序从 `stdin.txt` 读取输入，从 `stdout.txt` 读取输出，并给出判断结果。

```cpp
#include "checker.hpp"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream ifs("stdin.txt");
    ifstream ofs("stdout.txt");
    EventParser ep(ifs, ofs);
    try {
        Checker::checkAnswer(ep);
        cout << "Accepted" << endl;
    } catch (string& msg) {
        cout << msg << endl;
    }
    ifs.close();
    ofs.close();
    return 0;
}
```

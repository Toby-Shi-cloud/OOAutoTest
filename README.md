# README

=======
## 重要

请不要随便 `git add .` 或一键提交全部已更改文件。如想要提交，请认真检查自己修改的内容后，只提交该文件。

请配置 `config.json` 后使用。
配置方案参阅 [config 配置详细介绍](#config-配置详细介绍), 配置样例见 `config_template.json`。

## 关于提交新数据

直接加在 `data/data.dat` 后面，然后单独 `git add data/data.dat` 即可。

如果有其他想要改进的东西，可以提 issue 或者 pull requests

## 如何使用

- `get_jar.py`

> get_jar.py 将自动获取你的房间的所有人的代码，并编译并打包为 jar
>
> 在获取代码过程中，可能需要你提供账号和密码，以及本次作业 id，本次作业 id 可在打开一个作业后在地址栏查看：
> 如：`http://oo.buaa.edu.cn/assignment/418/intro`，则作业 id 为 `418`
>
> 注意：如果你不小心输入错误的账号或密码，并在下载中被提示 `Please login first!`，
> 则请删除 `network/login/cache/sso.cache` 重置 cookie

- `main.py`

> main.py 运行全部 data/data.dat 中的数据并将结果上传 lean cloud，并生成 score.csv。

- `run_test.py`

> run_test.py 将在本地运行数据。

- `cpp/judge.cpp`

> cpp 评测机请自行编译使用。
>
> **请注意：cpp 评测机目前不支持 HW3**
>
> **cpp 精度有限，注意不要使用过于庞大的数据**

## 其他代码介绍

- `gen/generator.py`

> 新版数据生成器，但是目前效果不理想

- `gen/generator.old.py`

> 旧版数据生成器，无法生成三角函数

- `gen/gen_xc.py`

> 肖灿哥哥写的数据生成器

- `util/checker.py`

> 检查输出是否符合形式化描述，且没有多余括号

- `util/judge.py`

> 用于运行程序，判断输出正确性，计算性能分

- `network.py`

> 用于与 lean cloud 交互

- `data/__init__.py`

> 用于读取 `data/data.dat`

- `auto_jar.py`

> 将指定目录下的所有 `zip` 解压并打包为 `jar`

------

## config 配置详细介绍：

### 总览

- `run_test.py` 相关的键 (keys) 有: `turn`, `sh_exec_list`, `judge`, `output`
  
- `main.py` 相关的键 (keys) 有: `judge`, `main_exec`

### 公共配置：

- `judge` - 选填 `sympy` 或 `cpp`。表示使用的评测机

### `run_test.py` 配置：

- `turn` - 选填 `-1`, `0`, 或 任意正整数 $x$
> `-1` 表示运行 `single_testcase.txt` 中的第一组数据 (请自行准备 `single_testcase.txt` 文件)  
> `0` 表示运行 `data/data.dat` 中的全部数据  
> $x$ 表示运行 `gen/gen_xc.py` 随机生成的 $x$ 组数据  

- `output` - 选填 `error`, `any`, 或 `none`
> `error` 表示 `run_test.py` 打印测试程序的输出, 当且仅当测试比对失败时  
> `any` 表示 `run_test.py` 任何时候都 **会** 打印测试程序的输出  
> `none` 表示 `run_test.py` 任何时候都 **不会** 打印测试程序的输出  

- `sh_exec_list` - 一个数组，其中每个元素表示一个欲测试的程序。具体写法参见[测试程序配置](#测试程序配置)

### `main.py` 配置：

- `main_exec` - 表示一个欲测试的程序。具体写法参见[测试程序配置](#测试程序配置)

### 测试程序配置

应为一个数组或一个 JSON 对象。推荐使用 JSON 对象配置。数组为兼容老版本配置。

- JSON 对象 配置方法
> 支持的键 (keys): `jar`, `class`, `exec`, 请选择三个键中的一个填写
> - `jar` 键: 填写你的 jar 路径 (绝对或相对)
> - `class` 键: 填写你的 MainClass 路径 (绝对或相对)
> - `exec` 键: 填写你的 可执行 文件 路径 (绝对或相对)
> 
> 举例：填写 `{ "jar": "./jar/Assassin.jar" }` 则会在 `./jar` 目录下，运行 `java -jar Assassin.jar`

- (兼容) 数组 配置方法
> 数组应该具有 2 个元素，第一个元素为 **运行指令**，第二个为 **运行目录**
> - 运行指令: 建议为数组，每个参数为其中一个元素，如填写 `[exec, arg1, arg2]` 则会在终端运行 `exec arg1 arg2`
> - 运行目录: 一个字符串表示路径 (绝对或相对)，指定上述指令在什么位置运行

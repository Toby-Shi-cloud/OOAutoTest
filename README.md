# README

=======
## 重要

请不要随便 `git add .` 或一键提交全部已更改文件。如想要提交，请认真检查自己修改的内容后，只提交该文件。

版本大更新！（at 2023.3.6 22:00）
请配置 `config.json` 后使用。
配置方案参阅 `config_templat.json`。

## 关于提交新数据

直接加在 `data/data.dat` 后面，然后单独 `git add data/data.dat` 即可。

如果有其他想要改进的东西，可以提 issue 或者 pull requests

## 如何使用

- `main.py`

> main.py 运行全部 data/data.dat 中的数据并将结果上传 lean cloud，并生成 score.csv。

- `run_test.py`

> run_test.py 将在本地运行数据。

- `cpp/judge.cpp`

> cpp 评测机请自行编译使用。

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

## 关于提交新数据

直接加在 `data/data.dat` 后面，然后单独 `git add data/data.dat` 即可。

如果有其他想要改进的东西，可以提 issue 或者 pull requests

## 如何使用

- `main.py`
> main.py 运行全部 data/data.dat 中的数据并将结果上传 lean cloud，并生成 score.csv。

- `run_test.py`
> run_test.py 将在本地运行全部 data/data.dat 中的数据。  
> run_test.py 的 SH_EXEC_LIST 可以添加多个程序供同时测试，填写方法：
```python
SH_EXEC_LIST = [
    ['args1', 'cwd1'],
    ['args2', 'cwd2'],
    ...
]
```
> 其中 `args` 表示运行程序的命令，如 `['java', '-jar', 'my.jar']`，或 `['zsh', 'run.sh']`，
> cwd 是程序运行的目录，建议改为 jar 所在路径或者 sh 所在路径。

- `data/data.dat`
> 以文本文件打开即可。为测试数据。每两组数据之间用空行隔开。

## 其他代码介绍

- `gen/generator.py`
> 新版数据生成器，但是目前效果不理想

- `gen/generator.old.py`
> 旧版数据生成器，无法生成三角函数

- `util/checker.py`
> 检查输出是否符合形式化描述，且没有多余括号

- `util/judge.py`
> 用于运行程序，判断输出正确性，计算性能分

- `network.py`
> 用于与 lean cloud 交互

- `data/__init__.py`
> 用于读取 `data/data.dat`

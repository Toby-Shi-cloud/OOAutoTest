import os
import json
from util.judge import judge_cpp, judge_sympy


def read_exec(cg):
    if isinstance(cg, list):
        return cg + [cg]
    elif isinstance(cg, dict):
        if 'jar' in cg.keys():
            cwd = os.path.dirname(cg['jar'])
            jar = os.path.basename(cg['jar'])
            return [['java', '-jar', jar], cwd, os.path.basename(cg['jar'])]
        elif 'class' in cg.keys():
            cwd = os.path.dirname(cg['class'])
            cls = os.path.basename(cg['class'])
            if len(cls) > 6 and cls[-6:] == '.class':
                cls = cls[:-6]
            return [['java', cls], cwd, os.path.basename(cg['class'])]
        elif 'exec' in cg.keys():
            cwd = os.path.dirname(cg['exec'])
            ecc = os.path.abspath(cg['exec'])
            return [[ecc], cwd, os.path.basename(cg['exec'])]
    raise KeyError('read exec config failed!')


config = json.load(open('config.json', encoding='utf-8'))

if config['judge'] == 'sympy':
    judge = judge_sympy
elif config['judge'] == 'cpp':
    judge = judge_cpp

exec_name = read_exec(config['main_exec'])

turn = config['turn']
output = config['output']
sh_exec_list = list(map(read_exec, config['sh_exec_list']))

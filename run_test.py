import json
import data
from gen.gen_xc import generate_expr
from util.judge import judge_sympy, judge_cpp, run_sh


config = json.load(open('config.json', encoding='utf-8'))
turn = config['turn']
output = config['output']
sh_exec_list = config['sh_exec_list']


def gen_many_data():
    gen_curturn = 0
    while gen_curturn < turn:
        gen_curturn += 1
        yield generate_expr()


# main
if __name__ == '__main__':
    if turn == -1:
        testcases = [open('single_testcase.txt').read()]
    elif turn == 0:
        testcases = data.TESTCASES
    elif turn > 0:
        testcases = gen_many_data()
    else:
        raise ValueError('Config Error: turn must be 0, -1 or positive integer')

    if config['judge'] == 'sympy':
        judge = judge_sympy
    elif config['judge'] == 'cpp':
        judge = judge_cpp
    else:
        raise ValueError('Config Error: judge must be sympy or cpp')

    curturn = 0
    for istr in testcases:
        print('#' + str(curturn) + ':')
        print('Input:')
        print(istr.strip())
        flag = False
        for sh_ in sh_exec_list:
            ostr = run_sh(sh_[0], istr, sh_[1])
            if output == 'any':
                print(sh_, 'Output:', ostr.strip())
            if not judge(istr, ostr):
                if output == 'error':
                    print('Output:', ostr.strip())
                print('Wrong Answer!', sh_[0])
                flag = True
        if flag:
            exit(-1)
        curturn += 1
        print()

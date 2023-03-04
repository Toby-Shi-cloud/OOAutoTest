import re
import data
from gen.gen_xc import generate_expr
from util.judge import judge, run_sh


# Setting
TURN = 0  # 0 run data/data.dat，0+ run gen_xc.py
SH_EXEC_LIST = [
    [['zsh', '../OOLab1/run.sh'], '../OOLab1'],
]


def gen_many_data():
    global TURN
    while TURN:
        TURN -= 1
        yield generate_expr()


# main
if __name__ == '__main__':
    turn = 0
    testcases = data.TESTCASES if TURN == 0 else gen_many_data()
    for istr in testcases:
        print('#' + str(turn) + ':')
        print('Input:')
        print(istr.strip())

        for sh_ in SH_EXEC_LIST:
            ostr = run_sh(sh_[0], istr, sh_[1])
            if not judge(istr, ostr):
                print('Output:', ostr)
                print('Wrong Answer!', sh_[0])
                exit(-1)
        turn += 1
        print()

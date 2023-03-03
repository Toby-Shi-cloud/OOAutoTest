import re
import data
from util.judge import judge, run_sh


# Setting
SH_EXEC_LIST = [
    [['zsh', '../OOLab1/run.sh'], '../OOLab1'],
]


# Parse Leading Zero
def parse_leading_zero(s: str):
    pattern = re.compile(r'(\D)0+(\d)')
    return pattern.sub(r'\g<1>\g<2>', s)


# main
if __name__ == '__main__':
    turn = 0
    for istr in data.TESTCASES:
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

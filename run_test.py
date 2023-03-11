import data
from colorama import Fore
from subprocess import TimeoutExpired
from func_timeout.exceptions import FunctionTimedOut

from gen.gen_xc import generate_expr
from util.judge import run_sh, OutputLimitExceeded
from util.read_config import judge, turn, output, sh_exec_list


def gen_many_data():
    gen_cur_turn = 0
    while gen_cur_turn < turn:
        gen_cur_turn += 1
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

    cur_turn = 0
    for istr in testcases:
        print('#' + str(cur_turn) + ':')
        print('Input:')
        print(istr.strip())
        flag = False
        for sh_ in sh_exec_list:
            try:
                ostr = run_sh(sh_[0], istr, sh_[1])
                if output == 'any':
                    print(sh_[2], 'Output:', ostr.strip())
                if not judge(istr, ostr):
                    if output == 'error':
                        print(sh_[2], 'Output:', ostr.strip())
                    print(Fore.RED + 'Wrong Answer!', sh_[2])
                    flag = True
            except RecursionError as e:
                print(Fore.RED)
                print(e)
                flag = True
            except OutputLimitExceeded as e:
                print(Fore.RED)
                print(e)
                flag = True
            except FunctionTimedOut as e:
                print(Fore.RED)
                print('Judge timed out after 60 seconds.' if str(e)[:2] != 'cpp' else e)
                flag = True
            except TimeoutExpired as e:
                print(Fore.RED)
                print(e)
                flag = True
            finally:
                print(Fore.RESET)
        if flag:
            exit(-1)
        cur_turn += 1
        print()

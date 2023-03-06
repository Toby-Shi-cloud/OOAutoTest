import re
import data
from gen.gen_xc import generate_expr
from util.judge import judge, run_sh


# Setting
TURN = 100  # 0 run data/data.dat，0+ run gen_xc.py, -1 run single test
SH_EXEC_LIST = [
    [['java', '-jar', 'Expr_expansion2.jar'], 'D:/.oo_file/hw2/Expr_expansion2/out/artifacts/Expr_expansion2_jar'],
]
SINGLE_TEST = '''3
h(z , x ,y)=+ +-00740-  ++002 * -0001 * +0000 * -0000 * +428* +002 -  -00549 * 00057341*+1* -00321 * +3- +61771 *+00819*-00082343*-3 * +000528 * +002 * -0149
f (y) =--+858 * +0002 *466* 338*+00829- +-000825 * -00921* -0001* 0001 * 002+  --0001 * 000677* 00605 *02 * y **+03 *y ** +00003+ -+048760 *+0002* +00062287 *+071721* y
g(x, z, y)= + -2*0000
- ++000984 *0078770 * -003*+0527*-003 * +0002 * 3+  ++001 *3 * -75442* -1'''


def gen_many_data():
    global TURN
    while TURN:
        TURN -= 1
        yield generate_expr()


# main
if __name__ == '__main__':
    turn = 0
    testcases = data.TESTCASES if TURN == 0 else gen_many_data() if TURN > 0 else [SINGLE_TEST]
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

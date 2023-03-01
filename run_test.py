import re
import sympy
from generator import generate_data
from subprocess import Popen, PIPE, TimeoutExpired


# Setting
MAX_TURN = 1
JAR_FILE_NAME_LIST = [
    'my.jar'
    # 'Alterego.jar',
    # 'Archer.jar',
    # 'Assassin.jar',
    # 'Berserker.jar',
    # 'Caster.jar',
    # 'Lancer.jar',
    # 'Saber.jar'
]


# Special Testcase
SPECIAL_CASE = [
    'y **+6*sin(cos((+-y **6*2*x--+1*y**+2*8*-2+--6*-5+--6*+5---8*y**+1)** 2)**8)*1--1+-3*x**+7+-+3*x**5+--3*x**6*+4+-y',
    '0*x**0',
]


# Parse Leading Zero
def parse_leading_zero(s: str):
    pattern = re.compile(r'(\D)0+(\d)')
    return pattern.sub(r'\g<1>\g<2>', s)


# main
if __name__ == '__main__':
    turn = 0
    while turn != MAX_TURN:
        print('#' + str(turn) + ':')
        istr = SPECIAL_CASE[turn] if turn < len(SPECIAL_CASE) else generate_data()
        print('Input:', istr.strip())

        for jar_file in JAR_FILE_NAME_LIST:
            with Popen(args=['java', '-jar', jar_file], encoding='utf-8', stdin=PIPE, stdout=PIPE, stderr=PIPE) as proc:
                try:
                    ostr, estr = proc.communicate(istr + '\n', 15)
                except TimeoutExpired:
                    proc.kill()
                    ostr, estr = ('', '')
            if '(' in ostr or ')' in ostr:
                print('Output:', ostr.strip())
                print(jar_file, 'Wrong Answer!')
                exit(-1)
            try:
                ifunc = sympy.sympify(parse_leading_zero(' ' + istr))
                ofunc = sympy.sympify(parse_leading_zero(' ' + ostr))
                if not ifunc.equals(ofunc):
                    print('Output:', ostr.strip())
                    print(jar_file, 'Wrong Answer!')
                    exit(-1)
            except ValueError:
                print('Output:', ostr.strip())
                print(jar_file, 'Wrong Answer!')
                exit(-1)
        turn += 1
        print()

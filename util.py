import re
import sympy
from subprocess import Popen, PIPE, TimeoutExpired


# Parse Leading Zero
def parse_leading_zero(s: str):
    """Delete leading zero"""
    pattern = re.compile(r'(\D)0+(\d)')
    return pattern.sub(r'\g<1>\g<2>', s)


def judge(s1: str, s2: str):
    """
    s1 - input str; s2 - output str;
    return True if s1 == s2 else False
    """
    if '(' in s2 or ')' in s2:
        return False
    try:
        ifunc = sympy.sympify(parse_leading_zero(' ' + s1))
        ofunc = sympy.sympify(parse_leading_zero(' ' + s2))
        return True if ifunc.equals(ofunc) else False
    except ValueError:
        return False


def run_jar(jar: str, istr: str):
    istr += '\n'
    with Popen(args=['java', '-jar', jar], encoding='utf-8', stdin=PIPE, stdout=PIPE, stderr=PIPE) as proc:
        try:
            ostr, estr = proc.communicate(input=istr, timeout=15)
        except TimeoutExpired:
            proc.kill()
            return ''
    return ostr


def get_grade(lp: int, lmin: int, base=1):
    x = lp / lmin
    if x <= 1:
        return 1.0 * base
    elif x >= 1.5:
        return 0.0 * base
    return (-31.8239 * x**4 + 155.9038 * x**3 - 279.2180 * x**2 + 214.0743 * x - 57.9370) * base

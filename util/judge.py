import re
import sys
import sympy
from colorama import Fore
from util.checker import check_all
from func_timeout import func_set_timeout
from subprocess import Popen, PIPE, TimeoutExpired
from func_timeout.exceptions import FunctionTimedOut


COMPARE_COUNT = 10000
RANDINT_RANGE = [-10000, 10000]
sys.setrecursionlimit(1000000)


class OutputLimitExceeded(Exception):
    pass


# Parse Leading Zero
def parse_leading_zero(s: str):
    """Delete leading zero"""
    pattern = re.compile(r'(\D)0+(\d)')
    return pattern.sub(r'\g<1>\g<2>', s)


@func_set_timeout(60)
def judge_sympy(s1: str, s2: str):
    """
    s1 - input str; s2 - output str;
    return True if s1 == s2 else False
    """
    s1 = parse_leading_zero(' ' + s1).strip()
    s2 = parse_leading_zero(' ' + s2).strip()
    if len(s2) > 200000:
        raise OutputLimitExceeded(f'Output Limit Exceeded! ({len(s2)} bytes)')
    fc = s1.split('\n')

    x = sympy.Symbol('x')
    y = sympy.Symbol('y')
    z = sympy.Symbol('z')
    sin = sympy.sin
    cos = sympy.cos
    dx = lambda expr: sympy.diff(expr, x)
    dy = lambda expr: sympy.diff(expr, y)
    dz = lambda expr: sympy.diff(expr, z)
    f, g, h = None, None, None
    eval_set = {
        'x': x, 'y': y, 'z': z,
        'sin': sin, 'cos': cos,
        'dx': dx, 'dy': dy, 'dz': dz,
        'f': f, 'g': g, 'h': h
    }
    for k in fc[1:-1]:
        k = k.replace(' ', '').replace('\t', '')
        matcher = re.match(r'^(?P<name>[fgh])\((?P<param>.*?)\)=(?P<expr>.*)$', k)
        if matcher is None:
            raise ValueError("Unknown Error: " + str(fc))
        expr_of_func = eval(matcher.groupdict()['expr'], eval_set)
        expr_of_func = sympy.simplify(expr_of_func)
        temp = eval('lambda ' + matcher.groupdict()['param'] + ': ' + str(expr_of_func), eval_set)
        if matcher.groupdict()['name'] == 'f':
            f = temp
            eval_set['f'] = f
        elif matcher.groupdict()['name'] == 'g':
            g = temp
            eval_set['g'] = g
        elif matcher.groupdict()['name'] == 'h':
            h = temp
            eval_set['h'] = h
        else:
            print(type(matcher.groupdict()['name']))
            print(matcher.groupdict()['name'])
            print(matcher.groupdict()['name'] == 'h')

    try:
        check_all(s2)
        cccc = eval(fc[-1], eval_set)
        if isinstance(cccc, int):
            return cccc == eval(s2)
        return True if cccc.equals(eval(s2)) else False
    except ValueError as e:
        print(e, file=sys.stderr)
        return False


@func_set_timeout(60)
def judge_cpp(s1: str, s2: str):
    s1 = s1.strip()
    s2 = s2.strip()
    if len(s2) > 1000000:
        raise OutputLimitExceeded(f'Output Limit Exceeded! ({len(s2)} bytes)')
    try:
        check_all(s2)
    except ValueError as e:
        print(e, file=sys.stderr)
        return False
    with Popen(args=['./cpp/judge'], encoding='utf-8', stdin=PIPE, stdout=PIPE, stderr=PIPE) as proc:
        try:
            ostr, estr = proc.communicate(input=s1 + '\n' + s2 + '\n', timeout=30)
        except TimeoutExpired:
            proc.kill()
            raise FunctionTimedOut('cpp judge time out after 30s')
    if ostr == 'Accepted!\n':
        return True
    print(Fore.RED + 'CPP JUDGE: ' + ostr.strip() + Fore.RESET, file=sys.stderr)
    return False


def run_jar(jar: str, istr: str):
    return run_sh(['java', '-jar', jar], istr)


def run_sh(sh: list[str], istr: str, cwd='.'):
    istr += '\n'
    with Popen(args=sh, cwd=cwd, encoding='utf-8', stdin=PIPE, stdout=PIPE, stderr=PIPE) as proc:
        try:
            ostr, estr = proc.communicate(input=istr, timeout=15)
        except TimeoutExpired as e:
            proc.kill()
            raise e
    if estr:
        print(Fore.RED + 'STDERR: ' + estr.strip() + Fore.RESET, file=sys.stderr)
    return ostr


def get_grade(lp: int, lmin: int, base=20):
    x = lp / lmin
    if x <= 1:
        return 1.0 * base
    elif x >= 1.5:
        return 0.0 * base
    return (-31.8239 * x**4 + 155.9038 * x**3 - 279.2180 * x**2 + 214.0743 * x - 57.9370) * base

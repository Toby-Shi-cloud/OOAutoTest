import re


def check_constant(s: str):
    matcher = re.match(r'^[+-]?\d+', s)
    return s[matcher.regs[0][1]:] if matcher is not None else None


def check_trigonometry(s: str):
    matcher = re.match(r'^(sin|cos)\(', s)
    if matcher is None:
        return None
    s = s[matcher.regs[0][1]:]
    s = check_factor(s, expr=True)
    matcher = re.match(r'\)(\*\*\+?\d+)?', s)
    return s[matcher.regs[0][1]:] if matcher is not None else None


def check_power(s: str):
    matcher = re.match(r'^[xyz](\*\*\+?\d+)?', s)
    return s[matcher.regs[0][1]:] if matcher is not None else None


def check_expr_factor(s: str):
    if s[0] != '(':
        return None
    s = check_expr(s[1:])
    if s is None:
        return None
    return s[1:] if s[0] == ')' else None


def check_factor(s: str, expr=False) -> str:
    r = check_constant(s)
    if r is not None:
        return r
    r = check_power(s)
    if r is not None:
        return r
    r = check_trigonometry(s)
    if r is not None:
        return r
    if expr:
        r = check_expr_factor(s)
        if r is not None:
            return r
    raise ValueError('Fatal: not a valid expression!')


def check_term(s: str) -> str:
    if s[0] == '+' or s[0] == '-':
        s = s[1:]
    s = check_factor(s)
    while s != '' and s[0] == '*':
        s = check_factor(s[1:])
    return s


def check_expr(s: str) -> str:
    if s[0] == '+' or s[0] == '-':
        s = s[1:]
    s = check_term(s)
    while s != '' and (s[0] == '+' or s[0] == '-'):
        s = check_term(s[1:])
    return s


def check_all(s: str):
    s = s.replace(' ', '').replace('\t', '')
    try:
        s = check_expr(s)
        if s != '':
            raise ValueError('Fatal: not a valid expression!')
    except IndexError:
        raise ValueError('Fatal: not a valid expression!')


if __name__ == '__main__':
    check_str = 'sin(x*x)'
    check_all(check_str)
    print('pass!')

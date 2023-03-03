from functools import reduce
from random import choice, randint, random, uniform


def choice_weight(data: dict):
    weight_sum = sum(data.values())
    if weight_sum < 1e-10:
        return list(data.keys())[0]
    threshold = uniform(0, weight_sum)
    cur = .0
    for key in data.keys():
        cur += data[key]
        if cur > threshold:
            return key
    raise RuntimeError('Impossible State:', data)


def spf(x: float):
    if x < 1e-10:
        return .0
    return x


class Generator:
    """ Methods in this class usually return a tuple[expr: str, length: int, cost: int] """

    settings = {
        'whitespace.list': list('  \t  '),
        'whitespace.length.min': 1,
        'whitespace.length.max': 2,
        'digit.list': list('0123456789'),
        'digit.length.max': 2,
        'digit.signal.list': ['+', '-', '-', '', ''],
        'variable.exponent.none': .2,
        'trigonometry.exponent.none': .2,
        'trigonometry.arg.weight': {
            'constant': 1,
            'variable': 2,
            'trigonometry': 2,
            'expression': 1,
        },
        'expression.exponent.none': .2,
        'factor.weight': {
            'constant': 1,
            'variable': 3,
            'trigonometry': 3,
            'expression': 2,
        },
        'expression.terms': 5,
    }

    def __init__(self, **kwargs):
        self.settings.update(kwargs)

    def white_space_term(self) -> str:
        length = randint(self.settings['whitespace.length.min'], self.settings['whitespace.length.max'])
        return reduce(lambda x, y: x + y, [choice(self.settings['whitespace.list']) for _ in range(length)], '')

    def integer_with_leading_zeros(self) -> tuple[str, int, int]:
        length = randint(1, self.settings['digit.length.max'])
        expr = reduce(lambda x, y: x + y, [choice(self.settings['digit.list']) for _ in range(length)], '')
        return expr, len(expr), max(1, reduce(lambda s, x: s + 1 if s or x != '0' else 0, expr, 0))

    def integer_with_signal(self) -> tuple[str, int, int]:
        res = self.integer_with_leading_zeros()
        sign = choice(self.settings['digit.signal.list'])
        return sign + res[0], len(sign) + res[1], len(sign) + res[1]

    def exponent(self, choice_list, sign=True) -> tuple[str, int, int]:
        if type(choice_list) is dict:
            index = choice_weight(choice_list)
        else:
            index = choice(choice_list)
        if sign:
            index_with_sign = choice(['+', '']) + choice(['0', '']) + index
        else:
            index_with_sign = index
        expr = '**' + self.white_space_term() + index_with_sign
        return expr, len(index_with_sign) + 2, int(index)

    def power_function(self) -> tuple[str, int, int]:
        if random() > self.settings['variable.exponent.none']:
            res = self.exponent('012345678')
            return choice('xyz') + self.white_space_term() + res[0], 1 + res[1], 1
        return choice('xyz'), 1, 1

    def trigonometry_function(self, limit_len, limit_cost) -> tuple[str, int, int]:
        """ to call this function, a limit_len greater than 10 is recommanded. """
        exp = None
        if limit_len > 10 and random() > self.settings['trigonometry.exponent.none']:
            exp = self.exponent({str(i): spf(limit_cost // 2 ** i) for i in range(9)})
            limit_len -= exp[1]
            limit_cost -= 2 ** exp[2]
        tri_func = choice(['sin', 'cos'])
        arg_type = choice_weight(self.settings['trigonometry.arg.weight'])
        if limit_len < 8:
            res = self.integer_with_signal()
            res = tri_func + '(' + res[0] + ')', 5 + res[1], 1 + res[2]
        elif arg_type == 'constant':
            res = self.integer_with_signal()
            res = tri_func + '(' + res[0] + ')', 5 + res[1], 1 + res[2]
        elif arg_type == 'variable':
            res = self.power_function()
            res = tri_func + '(' + res[0] + ')', 5 + res[1], 1 + res[2]
        elif arg_type == 'trigonometry':
            res = self.trigonometry_function(limit_len - 5, limit_cost - 1)
            res = tri_func + '(' + res[0] + ')', 5 + res[1], 1 + res[2]
        elif arg_type == 'expression':
            res = self.expression_factor(limit_len - 5, limit_cost - 1)
            res = tri_func + '(' + res[0] + ')', 5 + res[1], 1 + res[2]
        else:
            raise RuntimeError('Impossible State')
        if exp is None:
            return res
        else:
            return res[0] + exp[0], res[1] + exp[1], res[2] + 2 ** exp[2]

    def expression_factor(self, limit_len, limit_cost) -> tuple[str, int, int]:
        exp = None
        if random() > self.settings['expression.exponent.none']:
            exp = self.exponent({str(i): spf(limit_cost // 3 ** max(1, i)) for i in range(9)})
            limit_len -= exp[1]
            limit_cost **= 1 / max(1, exp[2])
        expr = self.expression(limit_len - 2, limit_cost)
        if exp is None:
            return '(' + expr[0] + ')', expr[1] + 2, max(2, expr[2])
        return '(' + expr[0] + ')' + exp[0], expr[1] + 2 + exp[1], max(2, expr[2]) ** max(1, exp[2])

    def factor(self, limit_len, limit_cost) -> tuple[str, int, int]:
        fac_type = choice_weight(self.settings['factor.weight'])
        if fac_type == 'constant':
            return self.integer_with_signal()
        elif fac_type == 'variable':
            return self.power_function()
        elif fac_type == 'trigonometry':
            return self.trigonometry_function(limit_len, limit_cost)
        elif fac_type == 'expression':
            return self.expression_factor(limit_len, limit_cost)
        else:
            raise RuntimeError('Impossible State')

    def term(self, limit_len, limit_cost) -> tuple[str, int, int]:
        if limit_len <= 3:
            return choice('0123456789'), 1, 1
        elif limit_cost <= 5:
            return self.power_function()
        facs = []
        leng = 0
        cost = 1
        while limit_cost > 5 and limit_len > 3:
            res = self.factor(limit_len, limit_cost)
            limit_len -= res[1]
            limit_cost //= res[2]
            facs.append(res[0])
            leng += res[1]
            cost *= res[2]
        return reduce(lambda x, y: x + '*' + y, facs), leng + len(facs) - 1, cost

    def expression(self, limit_len, limit_cost) -> tuple[str, int, int]:
        limit_len = int(limit_len * 0.9)
        limit_cost = int(limit_cost * 8)
        tems = []
        leng = 0
        cost = 0
        num = randint(1, self.settings['expression.terms'])
        for i in range(num):
            res = self.term(limit_len // num, limit_cost // num)
            tems.append(res[0])
            leng += res[1]
            cost += res[2]
        return reduce(lambda x, y: x + choice('+-') + y, tems), leng + len(tems) - 1, cost


if __name__ == '__main__':
    g = Generator()
    # sum_len = []
    # sum_cost = []
    # for ri in range(10000):
    #     out_res = g.expression(200, 50000)
    #     sum_len.append(out_res[1])
    #     sum_cost.append(out_res[2])
    # ave_len = sum(sum_len) / 10000
    # ave_cost = sum(sum_cost) / 10000
    # print('ave length:', ave_len)
    # print('delta length:', (sum([(x-ave_len)**2 for x in sum_len]) / 10000) ** 0.5)
    # print('ave cost:', ave_cost)
    # print('delta cost:', (sum([(x-ave_cost)**2 for x in sum_cost]) / 10000) ** 0.5)
    print('sample1:', *g.expression(20, 50000))
    print('sample2:', *g.expression(50, 50000))
    print('sample3:', *g.expression(200, 50000))

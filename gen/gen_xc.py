import random
import sys
sys.setrecursionlimit(1000000)

func_list = []
func_def_list = []
num_of_var = []
var_list = []

func_used = False
difficulty = 0
bracket_dep = 0
# 难度平衡全局变量


def generate_expr():
    res = rand_def_func()
    global var_list
    var_list = ['x', 'y', 'z']
    expr = rand_expr()
    while len(expr.replace(' ', '').replace('\t', '')) < 10 * random.randint(1, 5):
        op = random.choice(['+', '-'])
        expr += op + rand_space() + rand_term()
    func_list.clear()
    func_def_list.clear()
    num_of_var.clear()
    var_list.clear()
    return res + expr + '\n'


# 生成函数定义
def rand_def_func():
    global func_used
    global func_list
    func_used = True
    func_dic = ['f', 'g', 'h']
    func_list = random.sample(func_dic, random.randint(1, 3))
    func_num = len(func_list)
    def_str = str(len(func_list)) + '\n'
    for i in range(0, func_num):
        var_str = rand_var()
        num_of_var.append(len(var_list))
        func_def_list.append(rand_expr())
        def_str += func_list[i] + rand_space() + '(' + var_str[0:len(var_str)] + ')' + rand_space() + '=' + \
                   func_def_list[i] + '\n'
    func_used = False
    return def_str


# 生成1~3个变量用于定义
def rand_var():
    var_dic = ['x', 'y', 'z']
    global var_list
    var_list = random.sample(var_dic, random.randint(1, 3))
    var1 = var_list[0]
    var_str = rand_space() + var1 + rand_space()
    for i in range(1, len(var_list)):
        var = var_list[i]
        var_str += ',' + rand_space() + var + rand_space()
    return var_str


# 生成表达式
def rand_expr():
    op = random.choice(['+', '-'])
    final_expr = rand_space() + op + rand_term() + rand_space()
    for i in range(random.randint(0, 3)):
        op = random.choice(['+', '-'])
        final_expr += op + rand_space() + rand_term() + rand_space()
    return final_expr


def rand_term():
    op = random.choice(['+', '-'])
    final_term = rand_space() + op + rand_factor()
    for i in range(random.randint(0, 6)):
        final_term += rand_space() + "*" + rand_space() + rand_factor()
    return final_term


# 生成因子
def rand_factor():
    choice = random.choice(['var', 'const', 'expr'])
    ran_num = random.random()
    global bracket_dep
    if ran_num < 0.20 - 0.05 * bracket_dep - 0.02*difficulty:
        bracket_dep += 1
        return rand_expr_factor()
    elif ran_num < 0.60 + 0.03*difficulty:
        return rand_signed_int()
    else:
        return rand_var_factor()


# 生成表达式因子
def rand_expr_factor():
    expr = rand_expr()
    global bracket_dep
    global difficulty
    bracket_dep -= 1
    difficulty += 1
    if random.random() < 0.4:
        return '(' + expr + ')'
    return '(' + expr + ') ' + rand_index()


# 生成带符号整数
def rand_signed_int():
    sign = random.choice(['', '+', '-'])
    return sign + rand_int()


# 生成变量因子
def rand_var_factor():
    choice = random.choice(['pow', 'tri', 'def'])
    ran_num = random.random()
    global func_used
    global difficulty
    # if ran_num < 0.50:
    #     return rand_trig()
    # else:
    #     return rand_power()
    if ran_num < 0.33 - 0.03*difficulty and not func_used and len(func_list) != 0:
        difficulty += 1
        return rand_func()
    elif ran_num < 0.70 - 0.05*difficulty:
        return rand_trig()
    else:
        return rand_power()


# 生成幂函数
def rand_power():
    base = random.choice(var_list)
    if random.random() < 0.40:
        return base
    else:
        return base + rand_space() + rand_index()


# 生成三角函数
def rand_trig():
    func = random.choice(['sin', 'cos'])
    fun_str = func + rand_space() + '(' + rand_space() + rand_factor() + rand_space() + ')'
    if random.random() < 0.40:
        return fun_str
    else:
        return fun_str + rand_index()


# 生成函数调用
def rand_func():
    func_name = random.choice(func_list)
    var_num = num_of_var[func_list.index(func_name)]
    func_str = func_name + rand_space() + '(' + rand_space()
    func_str += rand_factor() + rand_space()
    for i in range(1, var_num):
        func_str += ',' + rand_space() + rand_factor() + rand_space()
    func_str += ')'
    return func_str


# 生成指数（0~8）可修改概率分布
def rand_index():
    rate = random.randint(0, 100)
    sign = random.choice(['', '+'])
    pre0 = '0' * random.randint(0, 5)
    global difficulty
    if rate > 95 + difficulty:
        difficulty += 1
        num = str(8)
    elif rate > 75 + 2*difficulty:
        num = str(random.randint(5, 8))
    elif rate > 40 + 3*difficulty:
        num = str(random.randint(3, 6))
    elif rate > 30:
        num = str(0)
    else:
        num = str(random.randint(0, 3))
    return '**' + rand_space() + sign + pre0 + num


# 生成随机整数，可以包含前导零
def rand_int():
    rate = random.randint(0, 100)
    pre0 = "0" * random.randint(0, 3)
    global difficulty
    if rate > 95 + difficulty:
        difficulty += 1
        return pre0 + str(random.randint(99999999, 99999999999))
    elif rate > 70 + 2*difficulty:
        return pre0 + str(random.randint(9999, 99999))
    elif rate > 60:
        return pre0 + str(random.randint(99, 999))
    elif rate > 30 + 3*difficulty:
        return pre0 + str(random.randint(0, 100))
    else:
        return pre0 + str(random.randint(0, 3))


# 生成空白项
def rand_space():
    rate = random.randint(0, 100)
    if rate > 95 + difficulty:
        return ' ' * 3 + '\t' * 3
    elif rate > 70 + 3*difficulty:
        return ' ' * 2 + '\t' * 1
    elif rate > 55:
        return ' ' * 1
    elif rate > 40:
        return '\t' * 1
    else:
        return ''


# generate_expr()
# print(rand_expr())

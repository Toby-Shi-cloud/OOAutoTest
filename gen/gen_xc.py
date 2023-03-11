import random
import sys
sys.setrecursionlimit(1000000)

func_list = []
func_def_list = []
num_of_var = []
var_list = []


func_used = False
expr_index = False
expr_used = False
deriv_used = False
def_func_now = 0
trig_deep = 0
difficulty = 0
bracket_dep = 0
diff_of_func = 0
# 难度平衡全局变量


def generate_expr():
    global var_list
    global difficulty
    global deriv_used
    global diff_of_func
    global def_func_now
    res = rand_def_func()
    var_list = ['x', 'y', 'z']
    difficulty = 0
    expr = rand_expr()
    while len(expr.replace(' ', '').replace('\t', '')) < 10 * random.randint(1, 5):
        op = random.choice(['+', '-'])
        expr += op + rand_space() + rand_term()
    func_list.clear()
    func_def_list.clear()
    num_of_var.clear()
    var_list.clear()
    deriv_used = False
    diff_of_func = 0
    def_func_now = 0
    return res + expr + '\n'


# 生成函数定义
def rand_def_func():
    global def_func_now
    global func_list
    global difficulty
    global diff_of_func
    func_dic = ['f', 'g', 'h']
    func_list = random.sample(func_dic, random.randint(0, 3))
    func_num = len(func_list)
    def_str = str(func_num) + '\n'
    difficulty = 0
    for i in range(0, func_num):
        var_str = rand_var()
        def_func_now = i
        num_of_var.append(len(var_list))
        func_def_list.append(rand_expr())
        diff_of_func += difficulty
        difficulty = 0
        def_str += func_list[i] + rand_space() + '(' + var_str[0:len(var_str)] + ')' + rand_space() + '=' + \
                   func_def_list[i] + '\n'
    diff_of_func = int(diff_of_func/4)
    def_func_now = -1
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
    max_num = 6 - 2 * expr_used - diff_of_func - 2 * (def_func_now >= 0) - 2 * func_used
    term_num = random.randint(0, 0 if max_num < 0 else max_num)
    for i in range(0, term_num):
        op = random.choice(['+', '-'])
        final_expr += op + rand_space() + rand_term() + rand_space()
    return final_expr


def rand_term():
    op = random.choice(['+', '-'])
    final_term = rand_space() + op + rand_factor()
    max_num = 4 - 2 * expr_used - diff_of_func - difficulty - 2 * (def_func_now >= 0) - 2 * func_used
    factor_num = random.randint(0, 0 if max_num < 0 else max_num)
    for i in range(0, factor_num):
        final_term += rand_space() + "*" + rand_space() + rand_factor()
    return final_term


# 生成因子
def rand_factor():
    choice = random.choice(['var', 'const', 'expr'])
    ran_num = random.random()
    global bracket_dep
    global deriv_used
    if ran_num < 0.1*(not deriv_used):
        deriv_used = True
        return rand_deriv_factor()
    elif ran_num < 0.25 + 0.15*(not deriv_used) - 0.07 * bracket_dep - 0.03*difficulty - 0.03 * diff_of_func - 0.02*(def_func_now >= 0) - 0.02*func_used:
        bracket_dep += 1
        return rand_expr_factor()
    elif ran_num < 0.40 + 0.2*(not deriv_used) + 0.03*difficulty + 0.03*diff_of_func:
        return rand_signed_int()
    else:
        return rand_var_factor()


# 生成导数因子
def rand_deriv_factor():
    var = random.choice(['x', 'y', 'z'])
    return 'd' + var + rand_space() + '(' + rand_expr() + ')'


# 生成表达式因子
def rand_expr_factor():
    global bracket_dep
    global difficulty
    global expr_index
    global expr_used
    expr_used = True
    difficulty += 1
    expr = rand_expr()
    bracket_dep -= 1
    expr_used = False
    if random.random() < 0.45 + 0.05*difficulty + 0.1*(def_func_now >= 0) + 0.05*diff_of_func:
        return '(' + expr + ')'
    else:
        expr_index = True
        final_expr = '(' + expr + ') ' + rand_index()
        expr_index = False
        return final_expr


# 生成带符号整数
def rand_signed_int():
    sign = random.choice(['', '+', '-'])
    return sign + rand_int()


# 生成变量因子
def rand_var_factor():
    choice = random.choice(['pow', 'tri', 'def'])
    ran_num = random.random()
    global def_func_now
    global difficulty
    # if ran_num < 0.50:
    #     return rand_trig()
    # else:
    #     return rand_power()
    if ran_num < 0.25 - 0.03*difficulty - 0.05*func_used - 0.05*diff_of_func and len(func_list) != 0 and def_func_now != 0:
        difficulty += 1
        return rand_func()
    elif ran_num < 0.5 - 0.07*difficulty - 0.1*trig_deep - 0.05*diff_of_func:
        return rand_trig()
    else:
        return rand_power()


# 生成幂函数
def rand_power():
    base = random.choice(var_list)
    if random.random() < 0.5 + 0.05*difficulty:
        return base
    else:
        return base + rand_space() + rand_index()


# 生成三角函数
def rand_trig():
    global trig_deep
    trig_deep += 1
    func = random.choice(['sin', 'cos'])
    fun_str = func + rand_space() + '(' + rand_space() + rand_factor() + rand_space() + ')'
    trig_deep -= 1
    if random.random() < 0.5 + 0.05*difficulty:
        return fun_str
    else:
        return fun_str + rand_index()


# 生成函数调用
def rand_func():
    global func_used
    global def_func_now
    def_func_now = len(func_list) if def_func_now == -1 else def_func_now
    func_used = True
    func_name = random.choice(func_list[0:def_func_now])
    var_num = num_of_var[func_list.index(func_name)]
    func_str = func_name + rand_space() + '(' + rand_space()
    func_str += rand_factor() + rand_space()
    for i in range(1, var_num):
        func_str += ',' + rand_space() + rand_factor() + rand_space()
    func_str += ')'
    func_used = False
    return func_str


# 生成指数（0~8）可修改概率分布
def rand_index():
    rate = random.randint(0, 100)
    sign = random.choice(['', '+'])
    pre0 = ''
    # pre0 = '0' * random.randint(0, 5)
    global difficulty
    # if rate > 95 + difficulty + 2*(def_func_now >= 0) + 2*func_used + (diff_of_func > 1) + (diff_of_func > 3) + 3*expr_index:
    #     difficulty += 2
    #     num = str(8)
    # elif rate > 75 + 3*difficulty + 5*(def_func_now >= 0) + 5*func_used + 5*(diff_of_func > 1) + 10*(diff_of_func > 3) + 10*expr_index:
    #     difficulty += 1
    #     num = str(random.randint(5, 8))
    # elif rate > 40 + 3*difficulty + 5*(def_func_now >= 0) + 3*func_used + 20*expr_index + 3*(diff_of_func > 1):
    #     num = str(random.randint(3, 5))
    # elif rate > 30 + 2*difficulty + 3*(def_func_now >= 0) + 2*func_used:
    #     num = str(random.randint(1, 3))
    # else:
    #     num = str(random.randint(0, 2))
    if rate > 30:
        num = str(random.randint(0, 2))
    else:
        num = str(random.randint(1, 4))
    return '**' + rand_space() + sign + pre0 + num


# 生成随机整数，可以包含前导零
def rand_int():
    rate = random.randint(0, 100)
    pre0 = ''
    # pre0 = "0" * random.randint(0, 3)
    global difficulty
    # return str(random.randint(0, 100))
    if rate > 95 + difficulty + 1*(def_func_now >= 0) + 1*func_used + (difficulty > 10):
        difficulty += 1
        return pre0 + str(random.randint(99999999, 99999999999))
    elif rate > 70 + 4*difficulty:
        return pre0 + str(random.randint(9999, 99999))
    elif rate > 60:
        return pre0 + str(random.randint(99, 999))
    elif rate > 30 + 5*difficulty:
        return pre0 + str(random.randint(0, 100))
    else:
        return pre0 + str(random.randint(0, 3))


# 生成空白项
def rand_space():
    rate = random.randint(0, 100)

    # if rate > 95 + difficulty:
    #     return ' ' * 3 + '\t' * 3
    # elif rate > 70 + 3*difficulty + 5*func_used:
    #     return ' ' * 2 + '\t' * 1
    # elif rate > 55:
    #     return ' ' * 1
    # elif rate > 40:
    #     return '\t' * 1
    # else:
    #     return ''

    return ""

# generate_expr()
# print(rand_expr())

from functools import reduce
from random import choice, randint, random

# Setting
MAX_SINGLE_WHITE_SPACE_LENGTH = 0
MAX_INTEGER_LENGTH = 1
PROBABILITY_OF_POWER_WITHOUT_EXPONENT = .3
PROBABILITY_OF_EXPRESSION_WITHOUT_EXPONENT = .3
PROBABILITY_OF_CONSTANT_FACTOR = .2
PROBABILITY_OF_VARIABLE_FACTOR = .4
MAX_FACTOR_PER_TERM = 2
MAX_TERM_PER_EXPRESSION = 2
MAX_NESTED_BRACKETS = 1

# Symbol Set
PLUS_MINUS = ['+', '-']
WHITE_SPACE = [' ', '\t']
DIGIT = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']
INDEX = ['0', '1', '2', '3'] * 5 + ['4', '5', '6'] * 2 + ['7', '8']
VARIABLES = ['x', 'y', 'z']


# Generators
def white_space_term():
    length = randint(0, MAX_SINGLE_WHITE_SPACE_LENGTH)
    return reduce(lambda x, y: x + y, [choice(WHITE_SPACE) for _ in range(length)], '')


def integer_with_leading_zeros():
    length = randint(1, MAX_INTEGER_LENGTH)
    return reduce(lambda x, y: x + y, [choice(DIGIT) for _ in range(length)], '')


def integer_with_signal():
    return choice(PLUS_MINUS + ['']) + integer_with_leading_zeros()


def exponent(smaller=False):
    if smaller:
        return '**' + white_space_term() + choice(['+', '']) + choice(['0', '']) + choice(INDEX[:20])
    return '**' + white_space_term() + choice(['+', '']) + choice(['0', '']) + choice(INDEX)


def power_function():
    if random() > PROBABILITY_OF_POWER_WITHOUT_EXPONENT:
        return choice(VARIABLES) + white_space_term() + exponent()
    return choice(VARIABLES)


def expression_factor(brackets):
    if random() > PROBABILITY_OF_EXPRESSION_WITHOUT_EXPONENT:
        return '(' + expression(brackets - 1) + ')' + white_space_term() + exponent()
    return '(' + expression(brackets - 1) + ')'


def factor(brackets):
    if brackets > 0:
        res = random()
        if res < PROBABILITY_OF_CONSTANT_FACTOR:
            return integer_with_signal()
        elif res < PROBABILITY_OF_CONSTANT_FACTOR + PROBABILITY_OF_VARIABLE_FACTOR:
            return power_function()
        else:
            return expression_factor(brackets)
    else:
        if random() < PROBABILITY_OF_CONSTANT_FACTOR \
                / (PROBABILITY_OF_CONSTANT_FACTOR + PROBABILITY_OF_VARIABLE_FACTOR):
            return integer_with_signal()
        else:
            return power_function()


def term(brackets, factors=MAX_FACTOR_PER_TERM):
    if random() < 1 / factors:
        return choice(PLUS_MINUS + ['']) + white_space_term() + factor(brackets)
    else:
        return term(brackets, factors - 1) + white_space_term() + '*' + white_space_term() + factor(brackets)


def expression(brackets, terms=MAX_TERM_PER_EXPRESSION):
    if random() < 1 / terms:
        return white_space_term() + choice(PLUS_MINUS + ['']) \
               + white_space_term() + term(brackets) + white_space_term()
    else:
        return expression(brackets, terms - 1) + choice(PLUS_MINUS) \
               + white_space_term() + term(brackets) + white_space_term()


# main function
def generate_data():
    return expression(MAX_NESTED_BRACKETS)


# pass rate test
def test():
    good = 0
    tot = 1000
    for _ in range(tot):
        if len(generate_data()) <= 200:
            good += 1
    return good / tot


if __name__ == '__main__':
    print(generate_data())
    print('Pass ratio:', test())

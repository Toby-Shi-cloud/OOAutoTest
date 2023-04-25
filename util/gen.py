import random

MAX_INPUT_LEN = 1000
MAX_NAME_LEN = 10
MAX_AGE = 200
MAX_VALUE = 100
used_id = set()


def to_rand_upper(s):
    return ''.join(random.choice([c.upper(), c]) for c in s)


def rand_name(maxlen=MAX_NAME_LEN):
    name = random.sample('abcdefghijklmnopqrstuvwxyz',
                         k=random.randint(1, maxlen))
    return to_rand_upper(''.join(name))


def rand_age():
    return random.randint(0, MAX_AGE)


def rand_value():
    return random.randint(0, MAX_VALUE)


def rand_id():
    if len(used_id) == 0 or random.choice([True, False]):
        id = random.randint(-10, 100)
        used_id.add(id)
    else:
        id = random.choice(list(used_id))
    return id


def gen_add_person():
    return 'ap {} {} {}'.format(rand_id(), rand_name(), rand_age())


def gen_add_relation():
    return 'ar {} {} {}'.format(rand_id(), rand_id(), rand_value())


def gen_query_value():
    return 'qv {} {}'.format(rand_id(), rand_id())


def gen_query_circle():
    return 'qci {} {}'.format(rand_id(), rand_id())


def gen_query_block_sum():
    return 'qbs'


def gen_query_triple_sum():
    return 'qts'


def gen(max_len=MAX_INPUT_LEN):
    len = random.randint(1, max_len)

    ops = [gen_add_person] * 10
    ops += [gen_add_relation] * 30
    ops += [gen_query_value] * 10
    ops += [gen_query_circle] * 10
    ops += [gen_query_block_sum] * 10
    ops += [gen_query_triple_sum] * 10

    return '\n'.join([gen_add_person() for _ in range(10)]+[op() for op in random.choices(ops, k=len-10)])


if __name__ == '__main__':
    print(gen())

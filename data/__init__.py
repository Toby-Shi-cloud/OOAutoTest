from random import shuffle


TESTCASES = []

dat = open('data/data.dat').readlines()
TESTCASES.append('')
for line in dat:
    if line == '' or line == '\n':
        TESTCASES.append('')
    else:
        TESTCASES[-1] += line
TESTCASES = list(filter(lambda x: x != '' and x != '\n', TESTCASES))

shuffle(TESTCASES)


if __name__ == '__main__':
    print(TESTCASES)
    print(len(TESTCASES))

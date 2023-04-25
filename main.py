from util.run import jar_thread
from util.cmp import cmp
from util.gen import gen
from colorama import Fore
import os
import threading

JAR_FOLDER_PATH = 'jar'
MODE = 'user_data'
CASES = 10
MAX_INPUT = 1000
CLEAN = True

input_path = os.path.join('data', 'input')
output_path = os.path.join('data', 'output')


def check(inst, i, jars):
    paths = []
    for jar in jars:
        path = os.path.join(
            output_path, os.path.basename(jar)[:-4],  f'{i}.txt')
        if i == -1:
            path = os.path.basename(jar)[:-4] + '_out.txt'
        paths.append(path)
        t=jar_thread(jar, inst, path)
        t.start()
    for t in threading.enumerate():
        if t is not threading.current_thread():
            t.join()
    r=cmp(paths)
    if r and CLEAN and i != -1:
        for path in paths:
            os.remove(path)
    print()
    return r


if __name__ == '__main__':
    if not os.path.exists(JAR_FOLDER_PATH):
        print(Fore.RED, end = '')
        print('No jar folder!')
        print(Fore.RESET)
        exit(0)
    jars=[]
    for file in os.listdir(JAR_FOLDER_PATH):
        if file.endswith('.jar'):
            jars.append(os.path.join(JAR_FOLDER_PATH, file))

    if MODE == 'rand_data':
        if not os.path.exists('data'):
            os.mkdir('data')
        if not os.path.exists(input_path):
            os.mkdir(input_path)
        if not os.path.exists(output_path):
            os.mkdir(output_path)
        for jar in jars:
            if not os.path.exists(os.path.join(output_path, os.path.basename(jar)[:-4])):
                os.mkdir(os.path.join(output_path, os.path.basename(jar)[:-4]))
        for i in range(CASES):
            inst=gen(MAX_INPUT)
            with open(os.path.join(input_path, f'{i}.txt'), 'w') as f:
                f.write(inst)
            print(f'TESTING CASE {i}')
            if check(inst, i, jars) and CLEAN:
                os.remove(os.path.join(input_path, f'{i}.txt'))

    elif MODE == 'user_data':
        input_file='input.txt'
        if not os.path.exists(input_file):
            input_file=input('Input file name: ')
            if not os.path.exists(input_file):
                print(Fore.RED, end = '')
                print('No input file!')
                print(Fore.RESET)
                exit(0)
        with open(input_file, 'r') as f:
            inst=f.read()
            check(inst, -1, jars)

from generator import generate_data
from subprocess import Popen, PIPE, TimeoutExpired


def run_jar(jar: str, istr: str):
    istr += '\n'
    with Popen(args=['java', '-jar', jar], encoding='utf-8', stdin=PIPE, stdout=PIPE, stderr=PIPE) as proc:
        try:
            ostr, estr = proc.communicate(input=istr, timeout=15)
        except TimeoutExpired:
            proc.kill()
            return ''
        return ostr


if __name__ == '__main__':
    for _ in range(40):
        istr = '$'
        ostr = '$' * 10001
        while len(ostr) > 10000 or len(istr) > 200:
            istr = generate_data()
            ostr = run_jar('my.jar', istr)
        print('\t' + repr(istr) + ',')

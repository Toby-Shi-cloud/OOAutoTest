import leancloud
import pandas as pd
from hashlib import md5
from alive_progress import alive_bar

from network import fetch
from data import TESTCASES
from util.judge import run_jar, judge, get_grade


def deal(s: str):
    if len(s) <= 1000:
        return s
    else:
        return s[:1000] + f"\n(Too long. {len(s)} bytes.)"


def main():
    print('===== Program Begin =====')
    jar_name = input('Please input your jar file name: ')

    print('===== Test Begin =====')
    df = pd.DataFrame(columns=['Testcase Hash', 'Status', 'Lp', 'Lmin', 'Performance', 'Input', 'Output'])
    try:
        with alive_bar(len(TESTCASES)) as bar:
            for istr in TESTCASES:
                ostr = run_jar(jar_name, istr)
                istr_hash = md5(istr.encode('utf-8')).hexdigest()
                if judge(istr, ostr):
                    lp = len(ostr.strip())
                    try:
                        lmin = fetch(istr_hash, lp)
                    except leancloud.LeanCloudError as e:
                        print('Error:', e)
                        print('Something went wrong...')
                        print('Maybe you can check your network and retry...')
                        break
                    df.loc[len(df.index)] = [istr_hash, 'Accepted', lp, lmin, get_grade(lp, lmin, 20), deal(istr), deal(ostr)]
                else:
                    df.loc[len(df.index)] = [istr_hash, 'Wrong Answer', None, None, 0.0, deal(istr), deal(ostr)]
                bar()
    except KeyboardInterrupt:
        print('KeyboardInterrupt!')
    df.to_csv('score.csv', index=False)
    print('===== Test End =====')

    print('See your score at score.csv')
    input('Press enter to exit')


if __name__ == '__main__':
    main()
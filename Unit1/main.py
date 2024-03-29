import sys
import json
import leancloud
import pandas as pd
from hashlib import md5
from alive_progress import alive_bar
from subprocess import TimeoutExpired
from func_timeout.exceptions import FunctionTimedOut

from data import TESTCASES
from network.cloud import fetch
from util.read_config import exec_name, judge
from util.judge import run_sh, get_grade, OutputLimitExceeded


def deal(s: str):
    if len(s) <= 1000:
        return s
    else:
        return s[:1000] + f"\n(Too long. {len(s)} bytes.)"


def main():
    print('===== Program Begin =====')

    print('===== Test Begin =====')
    df = pd.DataFrame(columns=['Testcase Hash', 'Status', 'Lp', 'Lmin', 'Performance', 'Input', 'Output'])
    try:
        with alive_bar(len(TESTCASES)) as bar:
            for istr in TESTCASES:
                ostr = run_sh(exec_name[0], istr, exec_name[1])
                istr_hash = md5(istr.encode('utf-8')).hexdigest()
                try:
                    if judge(istr, ostr):
                        lp = len(ostr.strip())
                        try:
                            lmin = fetch(istr_hash, lp)
                        except leancloud.LeanCloudError as e:
                            print('Error:', e)
                            print('Something went wrong...')
                            print('Maybe you can check your network and retry...')
                            break
                        df.loc[len(df.index)] =\
                            [istr_hash, 'Accepted', lp, lmin, get_grade(lp, lmin, 20), deal(istr), deal(ostr)]
                    else:
                        df.loc[len(df.index)] = [istr_hash, 'Wrong Answer', None, None, 0.0, deal(istr), deal(ostr)]
                except RecursionError as e:
                    print(e, file=sys.stderr)
                    df.loc[len(df.index)] = [istr_hash, 'Judge Recursion Overflow', None, None, None, deal(istr), deal(ostr)]
                except OutputLimitExceeded as e:
                    print(e, file=sys.stderr)
                    df.loc[len(df.index)] = [istr_hash, 'Output Limit Exceeded', None, None, 0.0, deal(istr), deal(ostr)]
                except FunctionTimedOut as e:
                    print('Judge timed out after 60 seconds.' if str(e)[:2] != 'cpp' else e, file=sys.stderr)
                    df.loc[len(df.index)] = [istr_hash, 'Judge Timeout', None, None, None, deal(istr), deal(ostr)]
                except TimeoutExpired as e:
                    print(e, file=sys.stderr)
                    df.loc[len(df.index)] = [istr_hash, 'Time Limit Exceeded', None, None, 0.0, deal(istr), None]
                bar()
    except KeyboardInterrupt:
        print('KeyboardInterrupt!')
    df.to_csv('score.csv', index=False)
    print('===== Test End =====')

    print('See your score at score.csv')
    input('Press enter to exit')


if __name__ == '__main__':
    main()

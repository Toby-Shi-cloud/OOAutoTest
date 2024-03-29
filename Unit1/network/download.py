import os
import json
import requests

from network.login import oo_login


def get_info(oo_cookies):
    homework_id = input('homework id: ')
    url = 'http:' + '//api.oo.buaa.edu.cn/homework/' + homework_id + '/mutual_test/room/self'
    r = requests.get(url, cookies=oo_cookies)
    return json.loads(r.text)


def download_zip() -> None:
    oo_cookies = oo_login()
    info = get_info(oo_cookies)
    if info['code'] != 0:
        raise ValueError('Fatal: ' + info['message'])

    if not os.path.exists('temp'):
        os.mkdir('temp')
    if not os.path.exists(os.path.join('temp', 'zip')):
        os.mkdir(os.path.join('temp', 'zip'))
    for member in info['data']['members']:
        name = member['alias_name_string']
        url = member['download_url']
        r = requests.get(url, cookies=oo_cookies)
        open(os.path.join('temp', 'zip', name + '.zip'), 'wb').write(r.content)

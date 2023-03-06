import re
import time
import random
import getpass
import requests
import urllib.parse
import http.cookiejar

from .utils.fake import request_resource
from .config.config import config, verify, proxies


def get_user_info() -> (str, str):
    username = input('Username: ')
    password = getpass.getpass('Password: ')
    return username, password


def init_session() -> (str, requests.Session):
    session = requests.session()
    session.cookies = http.cookiejar.LWPCookieJar(filename='./network/login/cache/sso.cache')
    index = session.get('https://sso.buaa.edu.cn', headers={
        **config['headers']['common'],
        **config['headers']['sso']['login']
    }, verify=verify, proxies=proxies)
    request_resource(session, index.text, 'https://sso.buaa.edu.cn', index.url)
    iframe = session.get('https://sso.buaa.edu.cn/cas/login-normal.html', headers={
        **config['headers']['common'],
        **config['headers']['sso']['iframe']
    }, verify=verify, proxies=proxies)
    request_resource(session, iframe.text, 'https://sso.buaa.edu.cn/cas', iframe.url)
    session.get('https://sso.buaa.edu.cn/banner.json', headers={
        **config['headers']['common'],
        **config['headers']['sso']['api']
    }, verify=verify, proxies=proxies)
    csrf_token = re.search('<input name="execution" value="(.+?)"/>', index.text).group(1)
    print('# info: init session for sso succeed')
    return csrf_token, session


def request_login() -> None:
    csrf_token, session = init_session()
    username, password = get_user_info()
    time.sleep(0.2 + random.random() / 2)
    session.get('https://sso.buaa.edu.cn/privacy/' + username, headers={
        **config['headers']['common'],
        **config['headers']['sso']['api']
    }, verify=verify, proxies=proxies)
    time.sleep(0.3 + random.random() / 2)
    session.post('https://sso.buaa.edu.cn/login', headers={
        **config['headers']['common'],
        **config['headers']['sso']['post']
    }, data={
        'username': username,
        'password': password,
        'submit': '登录',
        'type': 'username_password',
        'execution': csrf_token,
        '_eventId': 'submit'
    }, verify=verify, proxies=proxies, allow_redirects=False)
    session.cookies.save(ignore_discard=True, ignore_expires=True)
    print('# info: refresh sso login succeed')


def check_login() -> None:
    try:
        session = requests.session()
        sso_cookies = http.cookiejar.LWPCookieJar()
        sso_cookies.load('./network/login/cache/sso.cache', ignore_discard=True, ignore_expires=True)
        session.cookies = sso_cookies
        print('# info: sso cookies found, checking by uc login')
        index = session.get('https://uc.buaa.edu.cn', headers={
            **config['headers']['common'],
            **config['headers']['uc']['index']
        }, verify=verify, proxies=proxies)
        request_resource(session, index.text, 'https://uc.buaa.edu.cn', index.url)
        session.get('https://uc.buaa.edu.cn/api/register/rules?selfTimestamp=' + str(int(time.time() * 1000)), headers={
            **config['headers']['common'],
            **config['headers']['uc']['api']
        }, verify=verify, proxies=proxies)
        session.get('https://uc.buaa.edu.cn/api/uc/status?selfTimestamp=' + str(int(time.time() * 1000)), headers={
            **config['headers']['common'],
            **config['headers']['uc']['api']
        }, verify=verify, proxies=proxies)
        session.get('https://uc.buaa.edu.cn/api/reset/rules?selfTimestamp=' + str(int(time.time() * 1000)), headers={
            **config['headers']['common'],
            **config['headers']['uc']['api']
        }, verify=verify, proxies=proxies)
        route = urllib.parse.urlencode({'target': 'https://uc.buaa.edu.cn/#/user/login'})
        check = session.get('https://uc.buaa.edu.cn/api/login?' + route, headers={
            **config['headers']['common'],
            **config['headers']['uc']['api']
        }, verify=verify, proxies=proxies)
        if len(check.history) <= 1 or any(map(lambda x: x.status_code not in [301, 302], check.history)):
            print('# info: sso cookies check failed in uc login')
            request_login()
            check_login()
        else:
            print('# info: sso cookies check succeed in uc login')
    except FileNotFoundError:
        print('# info: sso cookies not found')
        request_login()
        check_login()


def sso_login() -> requests.Session:
    try:
        session = requests.session()
        sso_cookies = http.cookiejar.LWPCookieJar()
        sso_cookies.load('./network/login/cache/sso.cache', ignore_discard=True, ignore_expires=True)
        session.cookies = sso_cookies
        print('# info: sso cookies found')
        return session
    except FileNotFoundError:
        print('# info: sso cookies not found')
        request_login()
        return sso_login()

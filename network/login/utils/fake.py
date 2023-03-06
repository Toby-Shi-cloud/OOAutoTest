import re
import bs4
import json
import requests

from hashlib import sha1
from ..config.config import config, checksum, checksum_ignore_name, checksum_ignore_content, verify, proxies


def join_path(base: str, path: str) -> str or None:
    if not path.startswith('http'):
        base = base if base.endswith('/') else base + '/'
        path = path[1:] if path.startswith('/') else path
        return base + path
    else:
        return None


def get_checksum(html: str) -> str:
    for i in checksum_ignore_content:
        html = re.sub(i, '', html)
    return sha1(html.encode()).hexdigest()


def verify_checksum(path: str, html: str) -> None:
    for i in checksum_ignore_name:
        if re.match(i, path):
            return
    now_checksum = get_checksum(html)
    with open('./network/login/cache/checksums.json', 'r', encoding='utf-8') as file:
        content = json.loads(file.read())
    if content.get(path) is None or content[path] != now_checksum:
        print('# warning: checksum verify failed:')
        print('\tpath: ' + path)
        print('\tstd checksum: ' + ('null' if content.get(path) is None else content[path]))
        print('\tnow checksum: ' + now_checksum)


def request_path(session: requests.Session, path: str or None, referer: str) -> None:
    if path is not None:
        response = session.get(path, headers={
            **config['headers']['common'],
            'Referer': referer
        }, verify=verify, proxies=proxies)
        if checksum:
            verify_checksum(path, response.text)


def request_resource(session: requests.Session, html: str, base: str, referer: str) -> None:
    soup = bs4.BeautifulSoup(html, 'html.parser')
    if checksum:
        verify_checksum(referer, html)
    for i in soup.find_all('script'):
        try:
            request_path(session, join_path(base, i['src']), referer)
        except KeyError:
            pass
    for i in soup.find_all('link'):
        try:
            request_path(session, join_path(base, i['href']), referer)
        except KeyError:
            pass

import requests
import requests.utils

from .sso_login import sso_login


def oo_login(session=sso_login()):
    session.get('https' + '://sso.buaa.edu.cn/login?service=http%3A%2F%2Fapi.oo.buaa.edu.cn%2Fuser%2Fcas_login')
    return requests.utils.dict_from_cookiejar(session.cookies)

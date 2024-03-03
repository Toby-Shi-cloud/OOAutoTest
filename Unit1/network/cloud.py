"""
network & csv

HW: Object
HW.hash: str - the hash of input str
HW.lmin: int - the minimum length
"""

import leancloud


# 由于每届题目和测试数据不同，所以如果需要使用云存储判断你的答案性能分，请自行找新的云数据库
leancloud.init('AppID', 'AppKey')
HW3 = leancloud.Object.extend('WhatYouClassNamed')


def fetch(sh: str, length: int):
    query = HW3.query
    query.equal_to('hash', sh)
    data_list = query.find()
    if data_list:
        data = data_list[0]
        last_length = data.get('lmin')
        if last_length > length:
            data.set('lmin', length)
            data.save()
            return length
        return last_length
    else:
        data = HW3()
        data.set('hash', sh)
        data.set('lmin', length)
        data.save()
        return length

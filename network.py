"""
network & csv

HW: Object
HW.hash: str - the hash of input str
HW.lmin: int - the minimum length
"""

import leancloud


leancloud.init('IhwWUFBgg9AE1wYqpTCOcBIz-gzGzoHsz', '6mI1DvuYjGKOjoy9h12dMn98')
HW2 = leancloud.Object.extend('HW2')


def fetch(sh: str, length: int):
    query = HW2.query
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
        data = HW2()
        data.set('hash', sh)
        data.set('lmin', length)
        data.save()
        return length

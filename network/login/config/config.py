import json
import urllib3

urllib3.disable_warnings()

with open('./network/login/config/config.json', 'r', encoding='utf-8') as file:
    config = json.loads(file.read())

verify = not config['proxies']['enable']
proxies = None if verify else {
    'http': config['proxies']['http'],
    'https': config['proxies']['https']
}

checksum = config['checksums']['enable']
checksum_ignore_name, checksum_ignore_content = set(), set()
for i in config['checksums']['ignore']['name']:
    checksum_ignore_name.add(i)
for i in config['checksums']['ignore']['content']:
    checksum_ignore_content.add(i)

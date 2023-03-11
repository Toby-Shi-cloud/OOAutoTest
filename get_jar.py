import os
import json
from util.auto_jar import auto_jar
from network.download import download_zip


if __name__ == '__main__':
    download_zip()
    auto_jar(os.path.join('temp', 'zip'))

    try:
        config = json.load(open('config.json', encoding='utf-8'))
    except FileNotFoundError:
        config = json.load(open('config_template.json', encoding='utf-8'))
    if 'sh_exec_list.bk' not in config.keys():
        config['sh_exec_list.bk'] = config['sh_exec_list']
    config['sh_exec_list'] = []
    for jar_file in os.listdir('jar'):
        config['sh_exec_list'].append({ "jar": os.path.join('.', 'jar', jar_file) })
    json.dump(config, open('config.json', 'w', encoding='utf-8'), indent=4, ensure_ascii=False)

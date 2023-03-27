import os
import json
from util.auto_jar import auto_jar
from network.download import download_zip


if __name__ == '__main__':
    download_zip()
    auto_jar(os.path.join('temp', 'zip'), lib_path=None)

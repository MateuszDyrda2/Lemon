import json
import os
import re
import sys


def djb2(str):
    if not str:
        return 0

    hashstr = 5381
    for el in str:
        hashstr = ((hashstr << 5) + hashstr) + ord(el)

    return hashstr & 0xFFFFFFFF


def save_definitions(jsonPath, systems, filepath):
    if not os.path.exists(jsonPath):
        stages = {0: "earlyUpdate", 1: "update", 2: "lateUpdate", 3: "render"}
        with open(jsonPath, 'w+') as f:
            json.dump({'stages': stages, 'systems': {}, 'includes': [],
                      'components': {}}, f, indent=2)

    with open(jsonPath, 'r+') as f:
        data = json.load(f)
        for nameid, name in systems.items():
            data['systems'][nameid] = name

        f.seek(0)
        json.dump(data, f, indent=2)


def main(argv):
    jsonPath = argv[1]
    srcPath = argv[2]
    matchSys = re.compile('SYSTEM\((.*?)\)')
    systems = {}

    for line in open(srcPath, 'r'):
        for match in re.finditer(matchSys, line):
            string = match.group(1)
            name = [x.strip() for x in string.split(',')].pop(0)
            nameid = djb2(name)
            systems[nameid] = name

    with open(jsonPath, 'w+') as f:
        json.dump(systems, f)


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

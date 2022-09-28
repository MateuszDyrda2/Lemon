import json
import os
import re
import sys


def save_definitions(jsonPath, components, filepath):
    if not os.path.exists(jsonPath):
        stages = {0: "earlyUpdate", 1: "update", 2: "lateUpdate", 3: "render"}
        with open(jsonPath, 'w') as f:
            json.dump({'stages': stages, 'systems': {}, 'includes': [],
                      'components': {}}, f, indent=2)

    with open(jsonPath, 'r+') as f:
        data = json.load(f)
        for cname, field in components.items():
            data["components"][cname] = field

        if filepath not in data["includes"]:
            data['includes'].append(filepath)

        f.seek(0)
        json.dump(data, f, indent=2)


def main(argv):
    jsonPath: str = argv[1]
    srcPath: str = argv[2]
    matchComp = re.compile('\\[\\[lemon::component\\]\\]\\s+(\\w+)')
    matchField = re.compile('\\[\\[lemon::field\\]\\]\\s*(.*?(?=;|{))')
    components = {}
    includepath = srcPath.rpartition('include\\')[2]

    lastkey = ''
    for line in open(srcPath, 'r'):
        for match in re.finditer(matchComp, line):
            name = match.group(1)
            components[name] = {}
            lastkey = name

        for match in re.finditer(matchField, line):
            c = match.group(1).split()
            components[lastkey][c[1]] = c[0]

    save_definitions(jsonPath, components, includepath)


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

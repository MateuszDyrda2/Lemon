import json
import os
import re
import sys

def main(argv):
    jsonPath: str = argv[1]
    srcPath: str = argv[2]
    matchComp = re.compile('\\[\\[lemon::component\\]\\]\\s+(\\w+)')
    matchField = re.compile('\\[\\[lemon::field\\]\\]\\s*(.*?(?=;|{))')
    components = {}


    lastkey = ''
    for line in open(srcPath, 'r'):
        for match in re.finditer(matchComp, line):
            name = match.group(1)
            components[name] = {}
            lastkey = name

        for match in re.finditer(matchField, line):
            c = match.group(1).split()
            components[lastkey][c[1]] = c[0]

    with open(jsonPath, 'w+') as f:
        json.dump(components, f)

    print("generated {}".format(jsonPath))

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

import glob
import json
import os
import re
import sys
import time


def djb2(str):
    if not str:
        return 0

    hashstr = 5381
    for el in str:
        hashstr = ((hashstr << 5) + hashstr) + ord(el)

    return hashstr & 0xFFFFFFFF


def main(argv):
    startTime = time.time()
    srcPath = argv[1]
    outPath = argv[2]
    matchComp = re.compile('\\[\\[lemon::component\\]\\]\\s+(\\w+)')
    matchTag = re.compile('\\[\\[lemon::tag\\]\\]\\s+(\\w+)')
    matchField = re.compile('\\[\\[lemon::field\\]\\]\\s*(.*?(?=;|{))')
    matchSys = re.compile('SYSTEM\((.*?)\)')
    systems = {}
    components = {}
    stages = {0: "Early Update", 1: 'Update', 2: "Late Update", 3: "Render"}
    tags = []

    projectFiles = glob.glob(os.path.join(
        srcPath, '**/*.h'), recursive=True)

    for filename in projectFiles:
        lastComponent = ''
        for line in open(filename, 'r'):
            for match in re.finditer(matchSys, line):
                string = match.group(1)
                name = [x.strip() for x in string.split(',')].pop(0)
                nameid = djb2(name)
                systems[nameid] = name

            for match in re.finditer(matchComp, line):
                components[match.group(1)] = {}
                lastComponent = match.group(1)

            for match in re.finditer(matchField, line):
                c = match.group(1).split()
                components[lastComponent][c[1]] = c[0]

            for match in re.finditer(matchTag, line):
                tags.append(match.group(1))

    data = {
        'stages': stages,
        'systems': systems,
        'components': components,
        'tags': tags
    }

    with open(outPath, 'w') as outFile:
        outFile.write(json.dumps(data))

    print(f'Execution took: {time.time() - startTime}s', )


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

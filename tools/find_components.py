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
    matchField = re.compile('\\[\\[lemon::field\\]\\]\\s*(.*?(?=;|{))')
    matchSys = re.compile('SYSTEM\((.*?)\)')
    systems = []
    components = []
    stages = [
        {"id": 0, "name": "Early Update"},
        {"id": 1, "name": "Update"},
        {"id": 2, "name": "Late Update"},
        {"id": 3, "name": "Render"}]

    projectFiles = glob.glob(os.path.join(
        srcPath, '**/*.h'), recursive=True)

    for filename in projectFiles:
        for line in open(filename, 'r'):
            for match in re.finditer(matchSys, line):
                string = match.group(1)
                name = [x.strip() for x in string.split(',')].pop(0)
                nameid = djb2(name)
                if name == '_NAME':
                    continue
                systems.append({"id": nameid, "name": name})

            for match in re.finditer(matchComp, line):
                name = match.group(1)
                components.append({"name": name, "fields": []})

            for match in re.finditer(matchField, line):
                c = match.group(1).split()
                components[-1]["fields"].append({"name": c[1], "type": c[0]})

    data = {
        'stages': stages,
        'systems': systems,
        'components': components,
    }

    with open(outPath, 'w') as outFile:
        outFile.write(json.dumps(data))

    print(f'Execution took: {time.time() - startTime}s', )


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

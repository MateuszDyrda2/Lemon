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
    matchSys = re.compile('SYSTEM\((.*?)\)')
    matchComp = re.compile('LEMON_REFL\((.*?)\)')
    matchTag = re.compile('LEMON_TAG\((.*?)\)')
    systems = {}
    components = {}
    tags = []
    stages = {0: "earlyUpdate", 1: "update", 2: "lateUpdate", 3: "render"}

    projectFiles = glob.glob(os.path.join(
        srcPath, '**/*[!reflection].h'), recursive=True)

    for filename in projectFiles:
        for line in open(os.path.join(filename)):
            for m in re.finditer(matchSys, line):
                string = m.group(1)
                name = [x.strip() for x in string.split(',')].pop(0)
                nameid = djb2(name)
                systems[nameid] = name

            for m in re.finditer(matchComp, line):
                cfile = filename
                string = m.group(1)
                res = [x.strip() for x in string.split(',')]
                name = res.pop(0)
                components.setdefault(cfile, []).append({name: res})

            for m in re.finditer(matchTag, line):
                string = m.group(1)
                tags.append(string)

    # second pass

    componentDefs = {}
    for filename in components:
        for line in open(filename):

    data = {
        'stages': stages,
        'systems': systems,
        'components': components,
        'tags': tags
    }

    with open(outPath, 'w') as outFile:
        outFile.write(json.dumps(data))

    print('Execution took {} seconds'.format(time.time() - startTime))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Not enough parameters passed")
    else:
        main(sys.argv)

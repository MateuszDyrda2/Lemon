import glob
import json
import os
import re
import sys
import time


def main(argv):
    startTime = time.time()
    srcPath = argv[1]
    outPath = argv[2]
    matchSys = re.compile('SYSTEM\((.*?)\)')
    matchComp = re.compile('LEMON_REFL\((.*?)\)')
    matchTag = re.compile('LEMON_TAG\((.*?)\)')
    systems = []
    components = {}
    tags = []

    projectFiles = glob.glob(os.path.join(
        srcPath, '**/*[!reflection].h'), recursive=True)

    for filename in projectFiles:
        for line in open(os.path.join(filename)):
            for m in re.finditer(matchSys, line):
                string = m.group(1)
                name = [x.strip() for x in string.split(',')].pop(0)
                systems.append(name)
            for m in re.finditer(matchComp, line):
                string = m.group(1)
                res = [x.strip() for x in string.split(',')]
                name = res.pop(0)
                components[name] = res
            for m in re.finditer(matchTag, line):
                string = m.group(1)
                tags.append(string)

    data = {
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

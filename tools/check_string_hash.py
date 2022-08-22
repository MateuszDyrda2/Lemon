import os
import glob
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
    d = {}
    conficts = 0
    nbFiles = 0

    match = re.compile('hash_string\(\"(.*?)\"\)|\"(.*?)"_hs')

    srcPath = argv[1]

    projectFiles = glob.glob(os.path.join(srcPath, '**/*.cpp'), recursive=True) + \
        glob.glob(os.path.join(srcPath, '**/*.h'), recursive=True)

    for filename in projectFiles:
        nbFiles += 1
        for line in open(os.path.join(srcPath, filename)):
            for m in re.finditer(match, line):
                string = m.group(1) or m.group(2)
                strHash = djb2(string)
                if strHash in d:
                    if d[strHash] != string:
                        print("Confict detected! {} shares hash with {}".format(
                            string, d[strHash]))
                        conficts += 1
                else:
                    d[strHash] = string

    print('Checked {} file(s) and found {} conflict(s).'.format(nbFiles, conficts))
    print('Execution took {} seconds.'.format(time.time() - startTime))


if __name__ == "__main__":
    if len(sys.argv < 2):
        print('Too few arguments')
    else:
        main(sys.argv)

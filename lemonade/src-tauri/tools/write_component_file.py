import glob
import json
import sys
import time


def main(argv):
    startTime: float = time.time()
    srcPath: str = argv[1]
    outPath: str = argv[2]
    components = {}
    systems = {}
    stages = {0: "earlyUpdate", 1: "update", 2: "lateUpdate", 3: "render"}

    componentFiles = glob.glob(srcPath + "/*components*")
    systemFiles = glob.glob(srcPath + "/*system*")

    for componentFile in componentFiles:
        with open(componentFile, 'r') as f:
            data = json.load(f)
            components.update(data)

    for systemFile in systemFiles:
        with open(systemFile, 'r') as f:
            data = json.load(f)
            systems.update(data)

    with open(outPath, 'w+') as f:
        json.dump({'stages': stages, 'systems': systems,
                  'components': components}, f, indent=2)

    print('Execution took {:.2}s'.format(time.time() - startTime))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

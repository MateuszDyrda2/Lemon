import json
import sys
import time


def main(argv):
    startTime: float = time.time()
    srcPath: str = argv[1]
    outPath: str = argv[2]
    includeString: str = "#include <{}>\n"

    with open(srcPath, 'r') as f:
        data = json.load(f)
        componentList = data['components'].keys()
        includeList = [includeString.format(
            include) for include in data['includes']]

        fileString: str = """#pragma once
        #include <serialization/scene_serializer.h>
        {}
        namespace lemon {{
            static component_list_t<{}> componentList;
        }}
        """.format(''.join(includeList), ','.join(componentList))

        with open(outPath, 'w') as w:
            w.write(fileString)

    print('Execution took {:.2}s'.format(time.time() - startTime))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Not enough arguments')
    else:
        main(sys.argv)

import glob
import os
import sys
import time
import re


def main(argv):
    startTime = time.time()
    srcPath = argv[1]
    outPath = argv[2]
    match = re.compile('LEMON_REFL\((.*?)\)')
    tagMatch = re.compile('LEMON_TAG\((.*?)\)')
    components = {}
    tags = []
    includes = []

    projectFiles = glob.glob(os.path.join(
        srcPath, '**/*[!reflection].h'), recursive=True)

    for filename in projectFiles:
        isInclude = False
        for line in open(os.path.join(srcPath, filename)):
            for m in re.finditer(match, line):
                string = m.group(1)
                res = [x.strip() for x in string.split(',')]
                name = res.pop(0)
                components[name] = res
                isInclude = True
            for m in re.finditer(tagMatch, line):
                string = m.group(1)
                tags.append(string)
                isInclude = True

        if isInclude:
            includes.append(filename.split("include\\")
                            [1].replace("\\", "/"))

    includes = list(map(lambda x: "#include <{}>\n".format(x), includes))
    componentNames = ','.join(components.keys()) + ',' + ','.join(tags)
    fileContent = """ #pragma once

#include <serialization/scene_serializer.h>
#include <core/defines.h>
{}

namespace lemon {{
static component_list_t<{}> componentList;
}}""".format(''.join(includes), componentNames)

    with open(outPath, 'w') as f:
        f.write(fileContent)

    print('Execution took {} seconds'.format(time.time() - startTime))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Too few arguments')
    else:
        main(sys.argv)

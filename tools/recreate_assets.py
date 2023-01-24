import sys
import os
import json


def main(argv):
    paths = []
    names = ['textures', 'sounds', 'shaders', 'scripts', 'animations']
    for p in names:
        paths.append(os.path.join(argv[1], p))

    filepaths = {names[0]: [], names[1]: [],
                 names[2]: [], names[3]: [], names[4]: []}

    for p, n in zip(paths, names):
        for subdir, _, files in os.walk(p):
            for file in files:
                (_name, _path) = (file.rsplit(".", 1)[0], os.path.join(n, file))
                filepaths[n].append({"name": _name, "path": _path})

    with open(os.path.join(argv[1], 'assets.json'), 'w') as outfile:
        outfile.write(json.dumps(filepaths, indent=4))


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('Too few arguments')
    else:
        main(sys.argv)

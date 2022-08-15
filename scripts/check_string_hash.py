import os, glob, pathlib, re, time

def djb2(str):
    if not str:
        return 0
    
    hashstr = 5381
    for el in str:
        hashstr = ((hashstr << 5) + hashstr) + ord(el)

    return hashstr & 0xFFFFFFFF

def main():
    start_time = time.time()
    d = {}
    conficts = 0
    nb_files = 0

    match = re.compile('hash_string\(\"(.*?)\"\)|\"(.*?)"_hs')

    src_path = os.path.join(pathlib.PurePath(__file__).parent.parent, 'Sandbox', 'src')
    project_files = glob.glob(os.path.join(src_path, '**/*.cpp'), recursive=True) + glob.glob(os.path.join(src_path, '**/*.h'), recursive=True)

    for filename in project_files:
        nb_files += 1
        for line in open(os.path.join(src_path, filename)):
            for m in re.finditer(match, line):
                string = m.group(1) or m.group(2)
                strHash = djb2(string)
                if strHash in d:
                    if d[strHash] != string:
                        print("Confict detected! {} shares hash with {}".format(string, d[strHash]))
                        conficts += 1
                else:
                    d[strHash] = string

    print('Checked {} file(s) and found {} conflict(s).'.format(nb_files, conficts))
    print('Execution took {} seconds.'.format(time.time() - start_time))

if __name__ == "__main__":
    main()
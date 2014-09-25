#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Copy content from src to destination only if it doesn't
# exist in the destination

import os
import sys
import json
import shutil
import hashlib
from threading import Thread

isDir  = lambda p: p and os.path.isdir(p)
isPath = lambda p: p and os.path.isfile(p)

def getHashDigest(fPath):
    if isPath(fPath):
        with open(fPath, 'rb') as f: 
            digest = hashlib.md5(f.read()).hexdigest()

        return digest

def mapDigests(dirPath, hmap):
    for root, dirs, paths in os.walk(dirPath):
        joinedPaths = (os.path.join(root, path) for path in paths)
        for path in joinedPaths:
            digest = getHashDigest(path)
            hmap.setdefault(digest, []).append(path)
            print(path, digest)

def getNonExistant(primary, secondary):
    foreignToSecondary = []
    for digest in primary:
        if digest not in secondary:
            headList = primary[digest]
            if headList:
                foreignToSecondary.append(headList[0])
        
    return foreignToSecondary

def main():
    argc = len(sys.argv)
    if argc < 3:
        sys.stderr.write('Usage: <primary_dir> <secondary_dir>\n')
        sys.exit(-1)

    pdir, sdir = sys.argv[1:3]
    destination = sys.argv[3] if argc > 3 else sdir
    if not isDir(pdir):
        sys.stderr.write('Primary is not a directory\n')
    elif not isDir(sdir):
        sys.stderr.write('Secondary is not a directory\n')
    else:
        pmap = {}
        smap = {}
        pTh = Thread(target=mapDigests, args=(pdir, pmap))
        sTh = Thread(target=mapDigests, args=(sdir, smap))
        pTh.start()
        sTh.start()

        pTh.join()
        sTh.join()

        handleMerging(pmap, smap, destination)

def handleDirCreation(path):
    if not path:
        return 400, None
    elif os.path.isdir(path):
        return 409, path
    else:
        try:
            os.mkdir(path)
        except Exception as e:
            return 500, e
        else:
            return 200, path

def handleMerging(pmap, smap, destination):
    status, destPath = handleDirCreation(destination)
    if not (status == 200 or status == 409):
        return destPath # An error

    errd = []
    accessDenied = []

    passCount = 0
    foreignToSecondary = getNonExistant(pmap, smap)

    for i, path in enumerate(foreignToSecondary):
        if not os.access(path, os.R_OK):
            accessDenied.append(path) 
        else:
            try:
                shutil.copy(path, destPath)
            except Exception as e:
                errd.append((path, str(e),))
            else:
                sys.stdout.write("Successful Copy: index %d/%d\r"%(passCount, i))
                passCount += 1

    if errd:
        with open('errdCopy.json', 'w') as f:
            f.write(json.dumps(errd))
    if accessDenied:
        with open('accessDenied.json', 'w') as g:
            g.write(json.dumps(accessDenied))

    return passCount
       
if __name__ == '__main__':
    main()

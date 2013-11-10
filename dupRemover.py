#!/usr/bin/python3

# Author: Emmanuel Odeke <odeke@ualberta.ca>

import re
import os
import sys
import shutil
from hashlib import md5
from stat import S_ISREG, S_ISDIR

# Global variable
DEBUG = True

allCompile = re.compile("^[aA](ll)?", re.IGNORECASE)
yesCompile = re.compile("^[yY](es)?", re.IGNORECASE)

# Helper functions
pathExists = lambda aPath : aPath and os.path.exists(aPath)

# Affix path
affixPath = lambda parent, child : os.path.join(parent, child)

# Permission handlers
canRead = lambda path : pathExists(path) and os.access(path, os.R_OK)
canWrite = lambda path : pathExists(path) and os.access(path, os.W_OK)
canExecute = lambda path : pathExists(path) and os.access(path, os.X_OK)
canDelete = canWrite

# Helper functions for deleting
rmPath = os.unlink
rmDir = os.rmdir

def getMd5(path):
  # For debugging purposes
  # Function to return the md5 hex digest of a file 
  if pathExists(path):
    f = open(path, "rb") # Notice we'll be reading the data as bytes
    dBuffer = f.read()
    
    # Don't forget to close your file
    f.close
    return md5(dBuffer).hexdigest()

    # Even fancier
    # with f as open(path, "rb"):
    #   dBuffer = f.read()
    #   return md5.hexdigest(dBuffer)

def getStatDict(path):
  if pathExists(path):
    return os.stat(path)

def walkAndMap(dirPath, pathsFunctor, dirFunctor=None):
  pathGen = os.walk(dirPath)

  for root, dirs, paths in pathGen:
    # Applying the functor on each of the paths
    # More debugging
    # print(paths)
    affixedPaths = map(lambda p : affixPath(root, p), paths)
    functdPaths = map(lambda p : (p, pathsFunctor(p)), affixedPaths)

    if dirFunctor:
      functdDirs = map(lambda d : (d, dirFunctor(d)), dirs)
      returnCombo = (functdPaths, functdDirs)
    else:
      returnCombo = functdPaths

    yield returnCombo

def getFunctedPaths(path, pathFunctor):
  # Function to map the pathFunctor over all the paths
  # and return a dictionary/hashMap of each result 
  mappedGen = walkAndMap(path, pathFunctor)

  resultsDict = dict()
  while True:
    try:
      mapResults = mappedGen.__next__()
    except StopIteration as e:
      break
    else:
      for p, functdP in mapResults:
        resultsDict.setdefault(functdP, p)
        # Python is not a fan of mixing tabs and spaces

  return resultsDict

def getDuplicates(path1, path2):
  # Returns the intersection of path1 and path2
  # ie files that are in path1 and path2
  p1functdDict = getFunctedPaths(path1, getMd5)
  p2functdDict = getFunctedPaths(path2, getMd5)

  dupsFromP2 = list()
  for p2Md5Hash in p2functdDict:
    p1EquivPath = p1functdDict.get(p2Md5Hash, None)
    # Lookup time for dictionary elements should be O(1) 
    # if hashlist implementation has no collisions else 
    # it is O(n) but this rarely happens
    if p1EquivPath:
      dupTuple = (p2functdDict[p2Md5Hash], p1EquivPath)
      dupsFromP2.append(dupTuple)

  return dupsFromP2
     
def makeBackUp(path): 
  backUpPath = "%s_backUp"%(path)
  shutil.copytree(src=path, dest=backUpPath)

def delDupsFromP2(src, dest):
  duplicates = getDuplicates(src, dest)
  ignoreAll = False

  for destP, srcP in duplicates:
    if canDelete(destP):
       if not ignoreAll:
          promptForDel = input("Delete %s? [Y]es, [N]o or [A]ll? "%(destP))
          ignoreAll = (allCompile.match(promptForDel) != None)
           
       if ignoreAll or yesCompile.match(promptForDel):
          if DEBUG:
            print("Deleting %s\n"%(destP))

          rmPath(destP)

def main():
  argc = len(sys.argv) # Catching our argument count

  # Time to capture results from the commandline
  if (argc != 3):
    print("\033[33mDupPath deletor: [primaryPath secondaryPath] ")
    return
  else:
    duplist = getDuplicates(sys.argv[1], sys.argv[2])
    delDupsFromP2(sys.argv[1], sys.argv[2])

if __name__ == '__main__':
  main()
  # That's all folks

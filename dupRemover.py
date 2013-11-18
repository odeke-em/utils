#!/usr/bin/python3

# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Utility to remove duplicate paths from different directories
# For help: invoked the module with no arguments

import re
import os
import sys
import shutil # Module that contains the copy utilities
from hashlib import md5
from threading import Thread

# Global variable
DEBUG = True

allCompile = re.compile("^[a]+(ll)?", re.IGNORECASE)
yesCompile = re.compile("^[y]+(es)?", re.IGNORECASE)

stPrint = lambda msg: sys.stderr.write(msg+'\n') and sys.stderr.flush()

# Helper functions
pathExists = lambda aPath : aPath and os.path.exists(aPath)

# Affix path ie concatenate the parent to the child ie parent/child
affixPath = lambda parent, child : os.path.join(parent, child)

# Permission handlers
canRead = lambda path : pathExists(path) and os.access(path, os.R_OK)
canWrite = lambda path : pathExists(path) and os.access(path, os.W_OK)
canExecute = lambda path : pathExists(path) and os.access(path, os.X_OK)
canDelete = canWrite

# Helper functions for deleting
rmPath = os.unlink # For deleting regular files
rmDir = os.rmdir   # For deleting directories

class CustomThread(Thread):
  # Thread subclass that enables us to query
  # it for results from running a function
  def __init__(self, target, args):
    super().__init__()
    self.__funcToRun = target
    self.__args = args
    self.__results = None

  def run(self):
    # print("Args ", self.__args)
    self.__results = self.__funcToRun(*self.__args)

  def getResults(self):
    return self.__results

def getMd5(path):
  # Function to return the md5 hex digest of a file 
  if pathExists(path):
    f = open(path, "rb") # Notice we'll be reading the data as bytes
    dBuffer = f.read()
    
    # Don't forget to close your file
    f.close()
    return md5(dBuffer).hexdigest()

    # Even fancier
    # with f as open(path, "rb"):
    #   dBuffer = f.read()
    #   return md5.hexdigest(dBuffer)

def walkAndMap(dirPath, pathsFunctor, dirFunctor=None):
  # pathsFunctor : function to be applied on each of the paths
  # dirFunctor : function to be applied on each of the directories
  pathGen = os.walk(dirPath)

  for root, dirs, paths in pathGen:
    # Applying the functor on each of the paths
    # More debugging
    # stPrint(paths)
    affixedPaths = map(lambda p : affixPath(root, p), paths)
    functdPaths = map(lambda p : (p, pathsFunctor(p)), affixedPaths)

    if dirFunctor:
      functdDirs = map(lambda d : (d, dirFunctor(d)), dirs)
      returnCombo = (functdPaths, functdDirs)
    else:
      returnCombo = functdPaths

    yield returnCombo # yield keyword makes this function, a generator
                      # ie returns an iterator whose content is handed out
                      # per every invokation of '__next__()' -- think
                      # of a card dealer, you ask to be hit with the next card

def getFunctedPaths(path, pathFunctor):
  # Function to map the pathFunctor over all the paths
  # and return a dictionary/hashMap of each result 
  # Note: This will return a hashMap whose keys will be buckets
  # ie similar entries grouped together in one dictionary
  mappedGen = walkAndMap(path, pathFunctor)

  resultsDict = dict()
  while True:
    try:
      mapResults = mappedGen.__next__()
    except StopIteration as e:
      break
    else:
      for p, functdP in mapResults:
        if DEBUG: stPrint(p)
        elemBucket = resultsDict.get(functdP, {})

        # Placing all duplicates in the specific path in the same bucket
        # Assuming that each file name is unique map each path to itself
	# The use of a dict to handle duplicates and thus ease up with
        # insertions
        elemBucket.setdefault(p, p)

        # if len(elemBucket) > 1: print(elemBucket)

        resultsDict[functdP] = elemBucket

  return resultsDict

def getDuplicates(path1, path2):
  # Returns the intersection of path1 and path2
  # ie file buckets whose identifier is similar 
  # across both path1 and path2

  # Let's exploit some concurrency
  # def getFunctedPaths(path, pathFunctor):
  p1Thread = CustomThread(
    getFunctedPaths, (path1, getMd5)
  )
  p2Thread = CustomThread(
    getFunctedPaths, (path2, getMd5)
  )

  p1Thread.start()
  p2Thread.start()

  p1Thread.join()
  p2Thread.join()

  p1functdDict = p1Thread.getResults()
  p2functdDict = p2Thread.getResults()

  dupsFromP2 = list()
  for p2Md5Hash in p2functdDict:
    p1EquivBucket = p1functdDict.get(p2Md5Hash, None)
    # Lookup time for dictionary elements should be O(1) 
    # if hashlist implementation has no collisions else 
    # it is O(n) but this rarely happens
    if p1EquivBucket:
      dupTuple = (p2functdDict[p2Md5Hash], p1EquivBucket)
      dupsFromP2.append(dupTuple)

  # This list is going to contain buckets/hash lists of paths
  # of which each bucket contains paths of files that are
  # duplicates within the same directory
  return dupsFromP2
     
def makeBackUp(path): 
  backUpPath = "%s_backUp"%(path)
  shutil.copytree(src=path, dest=backUpPath)

def delDupsFromP2(src, dest):
  # Function to delete duplicates from 'dest' that are present in 'src'
  duplicates = getDuplicates(src, dest)
  ignoreAll = False

  for destBucket, srcBucket in duplicates:
    srcBucketIter = iter(srcBucket)
    # Just getting a sample element whose value will be used for display
    srcBucketHead =  srcBucketIter.__next__() 

    for destP in destBucket:
     if canDelete(destP):
       if not ignoreAll:
          if DEBUG: stPrint("%s and %s are the same"%(destP, srcBucketHead))
          promptForDel = input(
            "\033[32mDelete %s? [Y]es, [N]o or [A]ll? \033[00m"%(destP)
          )
          ignoreAll = (allCompile.match(promptForDel) != None)
           
       if ignoreAll or yesCompile.match(promptForDel):
          if DEBUG:
            stPrint("Deleting %s\n"%(destP))

          rmPath(destP)

def main():
  argc = len(sys.argv) # Catching our argument count

  # Time to capture arguments from the commandline
  if (argc != 3):
    stPrint("\033[33mDupPath deletor: [primaryPath secondaryPath] \033[00m")
    return
  else:
    primaryPath, secondaryPath = sys.argv[1], sys.argv[2]
    delDupsFromP2(primaryPath, secondaryPath)

if __name__ == '__main__':
  main()
  # That's all folks

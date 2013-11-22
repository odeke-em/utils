#!/usr/bin/python3

# Module to take out duplicates in a single path

import dupRemover
import random
import sys

DEBUG = True

def main():
  argc = len(sys.argv)
  if argc < 2:
    dupRemover.stPrint("\033[32mExpected a path: \033[00m")
    return

  target = sys.argv[1]
  hashedPaths = dupRemover.getFunctedPaths(target, dupRemover.getMd5)
  for p in hashedPaths:
    pBucket = hashedPaths[p]
    pBuckIter = iter(pBucket)
    headElem = pBuckIter.__next__()
    # print(headElem)

    # Remove the random pick from the bucket and purge the rest
    pBucket.pop(headElem)

    for mForDel in pBucket:
      if dupRemover.canDelete(mForDel):
         if DEBUG: 
            dupRemover.stPrint("%s and %s are the same"%(headElem, mForDel))
         promptForDel = input(
           "\033[32mDelete %s? [Y]es, [N]o or [A]ll? \033[00m"%(mForDel)
         )

         ignoreAll = (dupRemover.allCompile.match(promptForDel) != None)
           
         if ignoreAll or dupRemover.yesCompile.match(promptForDel):
            if DEBUG:
               dupRemover.stPrint("Deleted %s"%(mForDel))

            dupRemover.rmPath(mForDel)
  # print(hashedPaths)

if __name__ == '__main__':
  main()

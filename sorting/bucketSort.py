#!/usr/bin/python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

# We'll be doing all bucket resolutions in lower case

import sys

DEBUG = False
asciiA = ord('a')
asciiZ = ord('z')
ascii0 = ord('0')
ascii9 = ord('9')
azDiff = asciiZ - asciiA
_09Diff = ascii9 - ascii0

alphabetSize = azDiff + _09Diff + 1 # Extra space for unknown characters

def getIndex(ch):
  elemOrd = ord(ch)
  if (elemOrd >= asciiA and elemOrd <= asciiZ):
    return elemOrd - asciiA
  elif (elemOrd >= ascii0 and elemOrd <= ascii9):
    return (elemOrd - ascii0) + azDiff
  else: # Unknown to the alphabet in consideration
    return alphabetSize - 1
  
def buckSort(elems):
  maxElem = max(elems, key=lambda e: len(e[0]))
  bucketList = [list() for i in range(alphabetSize)]

  passCount = 0
  maxLen = len(maxElem[0])
  for index in range(maxLen - 1, -1, -1):
    while elems:
      e, l = elems.pop(0)
      if l <= index:
        bucketList[0].append((e, l))
      else:
        bIndex = getIndex(e[index])
        bucketList[bIndex].append((e, l))
    if DEBUG:
       print('bucketList ', bucketList)

    for b in bucketList:
      while b:
         elems.append(b.pop(0))

    if DEBUG:
       print("passCount ", passCount, elems)
    passCount += 1

preprocess = lambda iters,func : map(lambda e: (e, func(e)), iters)

def main():
  argc = len(sys.argv)
  if argc < 2:
    elemList = ["dumfries", "lucas", "bizery", "anointz12"]
  else:
    elemList = sys.argv[1:]

  print("Before ", elemList)
  lenPreprocessed  = list(preprocess(elemList, len))
  buckSort(lenPreprocessed)

  sortedElems = list(map(lambda e : e[0], lenPreprocessed))
  print("After ", sortedElems)

if __name__ == '__main__':
  main()

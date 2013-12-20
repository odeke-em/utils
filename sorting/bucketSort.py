#!/usr/bin/python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

# We'll be doing all bucket resolutions in lower case

import sys

DEBUG = False

capsA = ord('A')
capsZ = ord('Z')
lowerA = ord('a')
lowerZ = ord('z')
ascii0 = ord('0')
ascii9 = ord('9')

capsAZDiff = capsZ - capsA
lowerAZDiff = lowerZ - lowerA
_09Diff = ascii9 - ascii0

alphabetSize = capsAZDiff + lowerAZDiff + _09Diff + 1 # Extra space for unknown characters

def getIndex(ch):
  elemOrd = ord(ch)

  if (elemOrd >= ascii0 and elemOrd <= ascii9):
    return (elemOrd - ascii0)

  elif (elemOrd >= capsA and elemOrd <= capsZ):
    return _09Diff + (elemOrd - capsA)

  elif (elemOrd >= lowerA and elemOrd <= lowerZ):
    return _09Diff + lowerAZDiff + (elemOrd - lowerA)

  else: # Unknown to the alphabet in consideration
    return alphabetSize - 1
  
def buckSort(elems):
  maxElem = max(elems, key=lambda e: len(e[0]))
  bucketList = [list() for i in range(alphabetSize)]

  passCount = 0
  elemsLen = len(elems)
  maxLen = len(maxElem[0])
  for index in range(maxLen - 1, -1, -1):
    for i in range(elemsLen):
      e, l = elems[i]
      if l <= index:
        bucketList[0].append((e, l))
      else:
        bIndex = getIndex(e[index])
        bucketList[bIndex].append((e, l))
    if DEBUG:
       print('bucketList ', bucketList)

    elemsIndex = 0
    for bIndex in range(alphabetSize):
      for elem in bucketList[bIndex]:
         elems[elemsIndex] = elem
         elemsIndex += 1

      bucketList[bIndex] = [] # Reset the element, let GC handle the rest

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

  # print("Before ", elemList)
  lenPreprocessed  = list(preprocess(elemList, len))
  buckSort(lenPreprocessed)

  # sortedElems = list(map(lambda e : e[0], lenPreprocessed))
  for elem in lenPreprocessed:
    print(elem[0])
  # print("After ", sortedElems)

if __name__ == '__main__':
  main()

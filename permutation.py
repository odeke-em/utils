#!/usr/bin/python3

#Author: Emmanuel Odeke <odeke@ualberta.ca>
# Permutation generator

from profiler import profiled

cached = dict()

toString = lambda perms:"".join(perms)

def swap(lst, i, j):
  lstLen = len(lst)
  if (i >= lstLen) or (j >= lstLen): return -1

  temp = lst[i]
  lst[i] = lst[j]
  lst[j] = temp

  return 0

def cachingPerm(stringAsList):
  stLen = len(stringAsList)
  if (stLen <= 1): return stringAsList

  nPerms = 0
  storage = set()

  for i in range(0, stLen):
    iIndex = 0

    while (iIndex+1 < stLen):
      listAsStr = toString(stringAsList)
      storage.add(listAsStr)
      swap(stringAsList, iIndex, iIndex+1)
      iIndex += 1
      nPerms += 1

    head = stringAsList[0]
    rest = stringAsList[1:]

    restAsStr = toString(rest)
    cachedItemList = cached.get(restAsStr, None)

    if not cachedItemList: # Time to actually compute the values
      mergedSet = cachingPerm(rest)
      cachedItemList = list(map(lambda e: head+e, mergedSet))
      cached[listAsStr] = cachedItemList

      for elem in cachedItemList:
        storage.add(elem)

  return storage

def unCachingPerm(stringAsList):
  stLen = len(stringAsList)
  if (stLen <= 1): return stringAsList

  nPerms = 0
  storage = set()

  for i in range(0, stLen):
    iIndex = 0

    while (iIndex+1 < stLen):
      storage.add("".join(stringAsList))
      swap(stringAsList, iIndex, iIndex+1)
      iIndex += 1
      nPerms += 1

    head = stringAsList[0]
    rest = stringAsList[1:]

    mergedSet = unCachingPerm(rest)
    concatElems = map(lambda e: head+e, mergedSet)

    for elem in concatElems:
      storage.add(elem)

  return storage

def sortedPermutation(lst, functor):
  results = functor(lst)

  sortedResults = sorted(results)
  lenOfResults = len(sortedResults)

  return sortedResults, lenOfResults

@profiled
def cacheTest(lst):
  return sortedPermutation(lst, cachingPerm)

@profiled
def cacheLessTest(lst):
  functor = unCachingPerm
  return sortedPermutation(lst, unCachingPerm)

def main(testList):
  cached = cacheTest(testList)
  unCached = cacheLessTest(testList)
  
if __name__ == '__main__':
  main(['1', '2', 'm', '3', 'b', '5'])

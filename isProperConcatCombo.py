#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

def reverseLenMap(wList):
    # Return a map of length as keys and content as buckets
    #  where the buckets are hashmaps of word to a common sentinel
    lenToWordBucketMap = dict()
    for w in wList:
        if isinstance(w, str):
            wLen = len(w)
            bucket = lenToWordBucketMap.get(wLen, None)
            if bucket is None:
                bucket = dict()
                lenToWordBucketMap[wLen] = bucket
            bucket[w] = 0

    return lenToWordBucketMap

def isProperCombo(query, lenToWordBucketMap):
    startIndex=0
    maxIndex=len(query)
    sortedKeys = sorted(lenToWordBucketMap)
    qLen = len(query)
    while startIndex < maxIndex:
        for memLen in sortedKeys:
            travIndex = startIndex + memLen
            if travIndex > qLen: # Failed to match up here
                return False
            else:
                subStr = query[startIndex: travIndex]
                if subStr in lenToWordBucketMap[memLen]:
                    startIndex += memLen
                    break
    return True

def main():
    lenToWordBucketMap = reverseLenMap(['cat', 'emmanuel', 'tcormen', 'cormick'])
    query = 'catemmanuelcormicktcormenz'
    if isProperCombo(query, lenToWordBucketMap):
        print(query, '\033[92m is a valid join of various combinations\033[00m')
    else:
        print(query, '\033[91m is not a valid join of various combinations\033[00m')

if __name__ == '__main__':
    main()

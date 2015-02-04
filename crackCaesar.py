#!/usr/bin/python3

import sys
import functools

stdWrite = lambda msg: sys.stdout.write(msg) and sys.stdout.flush()
def editDistance(w1, w2):
  eD = 0

  for i in range(len(w1)):
    eD += (w2[i] == w1[i])

  return eD
    
def getLetterFrequencies(text):
  uniqueChars = set([elem for elem in text])

  frequencyDict = dict()
  textLength = len(text)

  for uniqueChar in uniqueChars:
    ltFrequency = 100*text.count(uniqueChar)/textLength

    frequencyDict[uniqueChar] = ltFrequency

  return frequencyDict

def main():
  nReads = sys.stdin.readline()
  dictionary = list()
  try:
    nReads = nReads.strip("\n")
    nReads = int(nReads)

    while nReads:
      nReads -= 1
      dictEntry = sys.stdin.readline().strip("\n")
      dictionary.append(dictEntry)

    sys.stdin.readline()

    ciphered = sys.stdin.readline().strip("\n")
  except:
    stdWrite("ERROR")
    return

  else:
    pass

  #print(dictionary)

  setOfDictLens = set([len(elem) for elem in dictionary])
  lenToLists = dict()
  for lenth in setOfDictLens:
    lenthV = filter(lambda elem: len(elem) == lenth, dictionary)
    #Pop each of those keys from dictionary
    lenToLists[lenth] = list(lenthV)

    list(map(lambda x:dictionary.remove(x), lenthV))
  #print(lenToLists)

  cipherMap = dict()

  idx = 0
  for elem in ciphered.split(" "):
    if not elem: continue

    cipherMap[idx] = elem.lower()
    idx += 1
    
  #print(cipherMap)
  alphaMap = dict()
  mapChars(lenToLists, cipherMap, alphaMap)

  keyList = sorted(cipherMap)

  stdWrite(" ".join([cipherMap[i].upper() for i in keyList]))

def mapChars(byLenDict, cipherMap, letterMap):
  #Start by finding the unique matches
  uniques = dict()

  for scrambled in cipherMap.values():
    scLen = len(scrambled)
    suggestions = byLenDict[scLen]
    if len(suggestions) != 1: continue
    uniques[scrambled] = suggestions[0]

  #print("UNIQUES", uniques)
  #Absolute unique elements found, start the mapping
  for uniq in uniques:
    key = uniques[uniq]
    for i in range(len(uniq)):
      letterMap[uniq[i]] = key[i]

  #print("LETTERMAP ", letterMap)

  for idx in cipherMap:
    key = cipherMap[idx] 
    newString = ""

    for ch in key:
      mapped = letterMap.get(ch, None)
      #print(ch, "MPD ", mapped)
      if mapped:
         newString += mapped
      else:
         newString += ch

    #Replacing the newString
    cipherMap[idx] = newString
    
  #for index in cipherMap:
  #  elem = cipherMap[index]
  #  elemLen = len(elem)

  #  accessList = byLenDict[elemLen]
  #  for suggestion in accessList:
  #    #print(elem, " suggestion ", suggestion)
  #    letterMap = list(map(lambda lt:(lt, suggestion[elem.find(lt)]), elem))

  #    toDict = dict()
  #    [toDict.setdefault(i[0], i[1]) for i in letterMap]

      #print(toDict)

  #cacheMisses = 0
 
  # Time to do some confidence ranking 
  for index in cipherMap:
    elem = cipherMap[index]
    elemLen = len(elem)
    confCount = 0
    if (elem not in byLenDict[elemLen]): #Start the ranking and edit distance
    #process
       iStart, iEnd = 0, elemLen-1
       while (iStart <= iEnd):
        confCount += (elem[iStart] in letterMap.values())
        confCount += (elem[iEnd] in letterMap.values())

        iStart += 1
        iEnd -= 1

    if not confCount: #Certainly fully matched
      continue

    if confCount <= elemLen:
      #Time to load suggestions
      suggestions = byLenDict.get(elemLen, [])
      editDistances = list()
      for suggest in suggestions:
        eD = editDistance(suggest, elem)
        editDistances.append((eD, suggest))

      maxMatch = max(editDistances, key=lambda k:k[0])
      if not maxMatch: continue #Bug jumping here LOL
      #Line up check
     
      topSuggest = maxMatch[1] 
      newString = ""
      for i in range(0, len(topSuggest)):
        sElem = topSuggest[i]
        oElem = elem[i]
        if sElem != oElem:
          letterMap[oElem] = sElem
        
        newString += sElem
      cipherMap[index] = newString    

  #print(letterMap)

if __name__ == '__main__':
  main()

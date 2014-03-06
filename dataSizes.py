#!/usr/bin/env python
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Quick/Dirty method for converting around data sizes

suffixes = ["KB", "MB", "GB", "TB", "PB"]
toDescription = lambda value, i=0 : "%2.2f%s"%(value, suffixes[i])\
   if (value/1024 < 1 or i >= len(suffixes) - 1) else toDescription(value/1024, i + 1)
def main():
  queries = [1000, 102334, 494944848400000000]
  for query in queries:
    print(query, toDescription(query))

if __name__ == '__main__':
  main()

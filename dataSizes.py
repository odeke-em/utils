#!/usr/bin/env python
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Quick/Dirty method for converting around data sizes

suffixes = ["B", "kB", "mB", "gB", "tB", "pB"]
toDescription = lambda value, i=0 : "%2.2f%s"%(value, suffixes[i])\
   if (value/1024 < 1 or i >= len(suffixes) - 1) else toDescription(value/1024, i + 1)
def main():
  queries = [8 * 19000000]
  for query in queries:
    print(query, toDescription(query))

if __name__ == '__main__':
  main()

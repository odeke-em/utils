#!/usr/bin/python3

# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Provides a decorator function that times a function
# The speed ups of caching are also demonstrated if this
# module's main function is invoked

import time
    
cache = dict() # Mapping n to n! [ its factorial value ]

def profiled(funct):
  def decorated(*args, **kwargs):
    begin = time.time()
    retValue = funct(*args)
    print(retValue)
    end = time.time()

    print("Time taken: %2.3f"%(end-begin))
    return retValue

  return decorated

@profiled
def cacheLessFactorial(n):
  if n <= 1: return 1

  return n * cacheLessFactorial(n-1)

@profiled
def cachingFactorial(n):
  if n <= 1: return 1

  cachedValue = cache.get(n, -1)
  if cachedValue != -1:
    return cachedValue

  else:
    value = cachingFactorial(n-1) * n
    cache[n] = value

    return value

def main():
  for i in range(200):
    print(cachingFactorial(i))

if __name__ == '__main__':
  main()

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

def nChooseK(n, k):
  f = cachingFactorial

  # (n k) = n!/(k!(n-k)!)
  nf = f(n)
  nfk = f(n-k)
  kf = f(k)

  return nf/(kf * nfk)

def callFib(n):
  cache.clear()

  return fibonacci(n)

@profiled
def fibonacci(n):
  if n <= 2:
    return 1

  memoized = cache.get(n, 0)
  if not memoized:
      # Trying to cut down as much function overhead as possible
     prev1 = cache.get(n-1, 0)
     if not prev1:
       prev1 = fibonacci(n-1)

     prev2 = cache.get(n-2, 0)
     if not prev2:
       prev2 = fibonacci(n-2)

     memoized = prev1 + prev2 
     cache[n] = memoized

  return memoized

def main():
  # for i in range(200):
  #   print(cachingFactorial(i))
  # n = 15
  # k = 6
  # res = nChooseK(n, k)
  # print(143*9)
  # print(res)

  for i in range(10000):
    fibonacci(i)
  print(fibonacci(10000))

if __name__ == '__main__':
  main()

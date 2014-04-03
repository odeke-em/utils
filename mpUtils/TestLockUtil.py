#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

import time
import multiprocessing
from threading import Thread

import LockUtil # Local module

gDict = dict() # Globally shared resource

@LockUtil.Locker
def sHingle():
    key = len(gDict)
    print('sHingle', key, gDict)
    gDict[2] = gDict.get(2, 0) - 1
    return gDict

@LockUtil.Locker
def pHingle():
    key = len(gDict)
    print('pHingle', key, gDict)
    gDict[2] = gDict.get(2, 0) + 2
    return gDict

@LockUtil.Retrieable
def retrier(func, *args, **kwargs):
    results = func(*args, **kwargs)
    return results

def main():            
    for i in range(40):
        if i & 1:
            func = sHingle
        else:
            func = pHingle

        th = Thread(target=retrier, args=(func,))
        th.start()

    m = 0
    while m < 20:
        time.sleep(1)
        m = gDict.get(2, 0)
        # print('Refresh', m)

if __name__ == '__main__':
    main()

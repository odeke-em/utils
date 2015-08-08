#!/usr/bin/env python3

import sys

a_ord   = ord('a')
max_ord = 1 + (ord('z') - a_ord)

def rotify(n, iterable):
    return ''.join(rotn(n, it) for it in iterable)

def rotn(n, a):
    a_ = '%s'%(a)

    if not a_.isalpha():
        return a_

    lowered = False
    if a_.isupper():
        lowered = True
        a_ = a_.lower()

    index = a_ord + (((ord(a_) - a_ord) + n) % max_ord)
    if lowered:
        index ^= 32

    return chr(index)

def main():
    argc = len(sys.argv)
    if argc < 2:
        return -1

    rest = sys.argv[1:]

    rot13 = lambda *args: rotify(13, *args)
    for arg in rest:
        print(rot13(arg))

if __name__ == '__main__':
    main()

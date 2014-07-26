#!/usr/bin/env python
# Author: Emmanuel Odeke <odeke@ualberta.ca>

import sys
import random
from optparse import OptionParser

random.seed(10)

def cliParser():
    parser = OptionParser()
    parser.add_option('-b', '--bottom', dest='bottom', default='0')
    parser.add_option('-c', '--count', dest='count', default='20000')
    parser.add_option('-t', '--top', dest='top', default='192738303082')
    parser.add_option('-o', '--outfile', dest='outfile', default='randIntegers.txt')

    return parser.parse_args()

def randSpit(ofPath, minV, maxV, count):
    if (minV < 0 or minV >= maxV):
        sys.stderr.write('\033[31mMinV must be unsigned and less than maxV\033[00m\n')
    else:
        with open(ofPath, 'w') as f:
            sys.stderr.write(
                '\033[47mWriting %d integers between %d and %d to %s\033[00m\n'%(
                    count, minV, maxV, ofPath
            ))
            for i in range(count):
                f.write('%s\n'%(random.randint(minV, maxV))) # str fmt to avoid integer conversion and loss of precision
                sys.stdout.write('Index: %d/%d\r'%(i, count))
        sys.stderr.write('\n\033[42mDone!\033[00m\n')

def main():
    args, options = cliParser()
    l, h = args.bottom, args.top
    n = args.count
    randSpit(args.outfile, int(args.bottom), int(args.top), int(args.count))

if __name__ == '__main__':
    main()

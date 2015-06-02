#!/usr/bin/env python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>

"""
# Sample Usage: tac <git_commit_logs> | ./gitac-rev.py reverse-chron-order.log
# In your shell
reverse_chrono () { type tac && tac $1 | ./gitac-rev.py $2; }
"""

import re
import sys
import traceback

atEOF = lambda p: p == ''

commitRegCompile = re.compile('^\s*commit\s+[a-z0-9]+', re.UNICODE|re.IGNORECASE)

def startOfGitCommit(p):
    return commitRegCompile.search(p)

def fflush(outf, newLine, *items):
    for item in items:
        outf.write('%s\n'%(item))

    if newLine:
        outf.write('\n')

    outf.flush()

def stacker(inf, outf, shouldFlushFunc):
    reading = True
    stack = []

    while reading:
        lineIn = inf.readline()
        if atEOF(lineIn):
            reading = False
            break

        stripped = lineIn.strip()
        stack.append(stripped)

        if shouldFlushFunc(stripped):
            revStack = stack[::-1]
            fflush(outf, True, *revStack)
            stack = []

def main():
    argc = len(sys.argv)

    outf = sys.stdout
    if argc >= 2:
        try:
            outf = open(sys.argv[1], 'w')
        except Exception: # For the purpose of making this cross version, ignore the exact error
            fflush(sys.stderr, True, "Error: %s"%(sys.argv[1]))
            traceback.print_exc()
            sys.exit(-1)

    stacker(sys.stdin, outf, startOfGitCommit)

if __name__ == '__main__':
    main()

#!/usr/bin/python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Logger to help with note taking eg during project collaboration

import os
import sys
from time import ctime

from optparse import OptionParser

getUserName = lambda : os.environ.get("LOGNAME", "Anonymous")

class Logger(object):
  def __init__(self, outPath, username):
    self.__fp = None
    self.__outPath = outPath
    self.__username = username
    self.__DEBUG = False

    self.__initLogFile()

  def setDebugLevel(self, debugStatus):
    self.__DEBUG = debugStatus

  def __initLogFile(self):
    self.__fp = open(self.__outPath, "a")
    if self.__fp == -1:
      raise Exception("Failed to initiated logfile %s"%(self.__outPath))

  def __logTime(self):
    self.__fp.write("%s [%s]\n"%(self.__username, ctime()))

  def __logMetaInfo(self):
    self.__fp.write("%s\n"%(os.environ))

  def writeEntry(self, msg):
    if not msg: return

    if self.__DEBUG:
      self.__logMetaInfo()

    self.__fp.write(msg)
    self.__fp.write("\n")
   
  def __flush(self):
    if hasattr("flush", self.__fp): 
      self.__fp.flush()
      return 0

    return -1

  def __call__(self, funct):
    def prettyFunc(*args, **kwargs):
      self.__logTime()
      self.writeEntry(funct(args, kwargs))

    return prettyFunc 

def cliParser():
  parser = OptionParser()
  parser.add_option('-u', '--username', dest="username", default=getUserName())
  parser.add_option('-t', '--target', dest="target", default="NOTES.txt")

  args, options = parser.parse_args()
  return args, options

def noteLogger(notesFile, username):
  @Logger(notesFile, username)
  def newNoteTaker(*args, **kwargs):
    notes = input("Your notes here: ")
    return notes

  return newNoteTaker

def main():
  parser = cliParser()
  args, options = parser

  username =  args.username
  notesFile =  args.target

  functor = noteLogger(notesFile, username)
  functor()

if __name__ == '__main__': 
  main()

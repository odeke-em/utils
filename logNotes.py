#!/usr/bin/python3
# Author: Emmanuel Odeke <odeke@ualberta.ca>
# Logger to help with note taking eg during project collaboration

import os
import sys
from time import ctime

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
    

@Logger("NOTES.txt", getUserName())
def main(*args, **kwargs):
  notes = input("Your notes here: ")
  return notes

if __name__ == '__main__': 
  main()

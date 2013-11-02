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
    self.__fp = -1
    self.__outPath = outPath
    self.__username = username
    self.__DEBUG = False

  def setDebugLevel(self, debugStatus):
    self.__DEBUG = debugStatus

  def __initLogFile(self):
    self.__fp = open(self.__outPath, "a")
    if self.__fp == -1:
      raise Exception("Failed to initiated logfile %s"%(self.__outPath))

  def writeToLog(self, msg):
    if self.__fp == -1:
      self.__initLogFile()

    self.__fp.write(msg)

  def __logTime(self):
    self.writeToLog("%s [%s]\n"%(self.__username, ctime()))

  def __logMetaInfo(self):
    self.writeToLog("%s\n"%(os.environ))

  def writeEntry(self, msg):
    # Logs the message 'msg' to the logfile
    # Note: Will not write to logfile unless the message is not EMPTY!
    if not msg: return

    if self.__DEBUG:
      self.__logMetaInfo()

    self.__logTime()
    self.writeToLog(msg+"\n")
   
  def __flush(self):
    if hasattr("flush", self.__fp): 
      self.__fp.flush()
      return 0

    return -1

  def __call__(self, funct):
    def prettyFunc(*args, **kwargs):
      self.writeEntry(funct(args, kwargs))

    return prettyFunc 

def cliParser():
  parser = OptionParser()
  parser.add_option('-u', '--username', dest="username", default=getUserName())
  parser.add_option('-n', '--noteFile', dest="noteFile", default="NOTES.txt")

  args, options = parser.parse_args()
  return args, options

def noteLogger(notesFile, username):
  @Logger(notesFile, username)
  def newNoteTaker(*args, **kwargs):
    try:
      notes = input("\033[92mYour notes here: \033[00m")
    except KeyboardInterrupt:
      return None

    return notes

  return newNoteTaker

def main():
  parser = cliParser()
  args, options = parser

  username =  args.username
  notesFile =  args.noteFile

  functor = noteLogger(notesFile, username)
  functor()

if __name__ == '__main__': 
  main()

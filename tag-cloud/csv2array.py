#!/usr/bin/python2

import sys
import csv

if len(sys.argv) != 4:
  print "Usage: %s <file.csv> <columnNumber> <columnType:num|str>" % sys.argv[0]
  sys.exit(1)

csvName, columnNumber, columnType = sys.argv[1], int(sys.argv[2]), sys.argv[3]

f = None
try:
  f = open(csvName)
except IOError as e:
  print e.strerror 
  sys.exit(1)

csvreader = csv.reader(f)
columns = csvreader.next()

if columnNumber >= len(columns):
  print "column number %s out of range" % columnNumber, len(columns)
  sys.exit(1)

wrapContent = lambda x: x
if columnType == "str":
  wrapContent = lambda x: "\"%s\"" % x

contents = []
for items in csvreader:
  if(columnNumber < len(items)):
    contents.append( wrapContent(items[columnNumber]) )
  else:
    print "column number %s out of range" % columnNumber, len(columns)

print "\n>>>Column name:", columns[columnNumber]
print "{", ", ".join(contents), "}" 

f.close()


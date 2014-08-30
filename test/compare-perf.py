#!/usr/bin/python

from collections import namedtuple

import math
import sys


if len(sys.argv) != 3:
  print "syntax: compare-perf.py <perf.log> <perf.log>"
  sys.exit()


Record = namedtuple('Record', ['timestamp', 'name', 'time', 'inSize', 'outSize', 'count'])


# Incremental computation of mean and standard deviation.
class Stat:
  def __init__(self):
    self.n  = 0
    self.m1 = 0
    self.m2 = 0

  def add(self, x):
    delta = x - self.m1
    self.n  += 1
    self.m1 += delta / self.n
    self.m2 += delta * (x - self.m1)

  def mean(self):
    return self.m1

  def var(self):
    return self.m2 / (self.n - 1)

  def stddev(self):
    return math.sqrt(self.var())


def parseLog(f):
  if f.readline() != 'timestamp\tname\ttime(s)\tin-size(B)\tout-size(B)\tcount\n':
    raise Exception("unexpected header")

  data = []
  for line in f:
    p = line.split('\t')
    timestamp =   int(p[0])
    name      =       p[1]
    time      = float(p[2])
    inSize    =   int(p[3])
    outSize   =   int(p[4])
    count     =   int(p[5])
    data.append(Record(timestamp, name, time, inSize, outSize, count))
  return data


def compareLogs(l1, l2):
  if len(l1) != len(l2):
    print "record count mismatch"
    return

  n = len(l1)

  stat = Stat()

  for r1, r2 in zip(l1, l2):
    if r1.name != r2.name:
      print "name mismatch"
      return
    if r1.inSize != r2.inSize:
      print "in-size mismatch"
      return
    if r1.outSize != r2.outSize:
      print "out-size mismatch"
      return
    stat.add(r1.time - r2.time)

  print "{0:.3}({1:.3})".format(stat.mean(), stat.stddev())


with open(sys.argv[1], "r") as f1:
  with open(sys.argv[2], "r") as f2:
    compareLogs(parseLog(f1), parseLog(f2))

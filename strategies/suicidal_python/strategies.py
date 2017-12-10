#!/usr/bin/env python

import sys

isAnt = False

for line in sys.stdin:
    line = line.strip()
    if line == "END":
        if isAnt:
            print("SUICIDE")
        print("END", flush=True)
        isAnt = False
    elif line == "BEGIN ANT":
        isAnt = True

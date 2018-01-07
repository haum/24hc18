#!/usr/bin/env python

import sys

isAnt = False
memory = ''

for line in sys.stdin:
    line = line.strip()
    if line.startswith("MEMORY"):
        memory = line
    if line == "END":
        if isAnt:
            #print("PUT_PHEROMONE 12")
            #if memory == 'MEMORY 0 0':
            print("EXPLORE")
            pass
        else:
            if memory == 'MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0':
                print("ANT_NEW 0")
                print("SET_MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1")
            elif memory == 'MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1':
                print("ANT_OUT 0 0 0")
                print("SET_MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 2")
        print("END", flush=True)
        isAnt = False
    elif line == "BEGIN ANT":
        isAnt = True

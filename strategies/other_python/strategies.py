#!/usr/bin/env python

"""
# scenario.cfg

MAXTEAMS 1
DURATION 1
NEST_POSITION 0 20 0
NEST_POPULATION 0 10 5
NEST_POPULATION 0 15 5
NEST_FOOD 0 20
FOOD 25 5 5 1 200 1000000 0

"""

import sys

isAnt = False
isNest =False
new_last = False

for line in sys.stdin:
    #line.upper()
    line = line.strip()

    if isNest: # on recupere les infos sur le nid
        if line:
            line = line.split()
            if line[0] == "STOCK":
                pass
            elif line[0] == "MEMORY":
                pass
            elif line[0] == "ANT_COUNT": # infos fourmis
                if ((line[1] == "10") & (int(line[2]) > 0)): #si type 10 et qu'il y'en a encore
                    if new_last:
                        repNest ="ANT_OUT 10 0 0 0" # elle sort du nid
                    else:
                        repNest ="ANT_NEW 10" # on crée une fourmi
                    new_last = False if new_last else True
            elif line[0] == "END":
                isNest =False
                print (repNest)
                print("END", flush=True)
    elif isAnt: # on recupere les infos sur la fourmi
        if line:
            line = line.split()
            if line[0] == "TYPE":
                pass
            elif line[0] == "MEMORY":
                if int(line[1]) < 10: # la fourmis explore 10 fois
                    steep_move = int(line[1])+1
                    repAnt = repAnt + f"SET_MEMORY {str(steep_move)} {line[2]} \n" # on stock le nombre de move
                    repAnt = repAnt + f"EXPLORE"
                else:
                    repAnt = repAnt + f"SUICIDE" # si deja 1 explorations SUICIDE
            elif line[0] == "END":
                isAnt = False
                print (repAnt)
                print("END", flush=True)


    if line == "BEGIN ANT":
        isAnt = True
        repAnt = ""
    elif line == "BEGIN NEST":
        isNest = True
        repNest = ""


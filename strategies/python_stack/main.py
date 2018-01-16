#! /usr/bin/env python

from utils import Ant, Nest
from strats import nest_strat, ant_strat
import sys

lines = []
for line in sys.stdin:
    line = line.strip()

    if line.startswith('BEGIN'):
        _, entity = line.split(' ')
        entity_class = Nest if entity == 'NEST' else Ant
        lines = []
    elif line == 'END':
        e = entity_class(lines)
        if type(e) == Nest:
            output = nest_strat(e)
        else:
            output = ant_strat(e)
        for l in output: print(l)
        print('END', flush=True)
        del e
    else:
        lines.append(line)

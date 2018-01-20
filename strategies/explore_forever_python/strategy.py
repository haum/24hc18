#!/usr/bin/env python3
import sys

N = 9
is_nest = False
output = []
for line in sys.stdin:
    line = line.strip()
    if line == 'BEGIN NEST':
        is_nest = True
    elif line == 'BEGIN ANT':
        is_nest = False
    elif is_nest and line.startswith('MEMORY 0'):
        output += ['ANT_NEW 0', 'SET_MEMORY 1'+' 0'*19]
    elif is_nest and line.startswith('MEMORY 1'):
        output += ['ANT_OUT 0 0 0 0', 'SET_MEMORY 2'+' 0'*19]
    elif not is_nest and line.startswith('MEMORY') and int(line.split(' ')[1]) > N:
        output += ['PUT_PHEROMONE 0', 'SET_MEMORY 0 0']
    elif not is_nest and line.startswith('MEMORY'):
        output += ['EXPLORE', f'SET_MEMORY {int(line.split(" ")[1])+1} 0']
    elif line == 'END':
        print('\n'.join(output+['END']), flush=True)
        output = []

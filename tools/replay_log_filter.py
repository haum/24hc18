#!/usr/bin/env python

import sys

step = True
with open('replay.log') as f:
    for line in f:
        if line[0] != '@':
            continue
        line = line[2:].strip()
        print(line, flush=True)
        print('@', line, file=sys.stderr, flush=True)
        if line == 'END' and step:
            action = sys.stdin.readline().strip()
            if action == 'q':
                break;
            elif action == 'c':
                step = False;

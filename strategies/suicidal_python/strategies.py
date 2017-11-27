#!/usr/bin/env python

import sys

while True:
    line = sys.stdin.readline().strip()
    if line == "END":
        print("END", flush=True)
    elif line == "BEGIN ANT":
        while line != "END":
            line = sys.stdin.readline().strip()
        print("SUICIDE")
        print("END", flush=True)

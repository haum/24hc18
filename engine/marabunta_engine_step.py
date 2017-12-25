#!/bin/env python3

import sys
import subprocess

try:
    import gdb
except:
    pass

lastcmd = ""
def breakpointHandler (event):
    global lastcmd
    frame = gdb.newest_frame()
    fn = frame.name().split('(')[0]
    if fn == "ScenarioBase::run":
        gdb.execute("call clock.setOneShot()")
        gdb.execute("continue")
    elif fn == "breakAnchor":
        anchor = frame.read_var("name").string()
        while True:
            print("Debugger stopped at anchor", anchor,
                  "(h for help):", flush=True, end=' ')
            cmd = sys.stdin.readline().strip()
            if cmd == "":
                cmd = lastcmd
            else:
                lastcmd = cmd;
            if cmd == "h":
                print("h    Print help")
                print("c    Continue")
                print("q    Quit")
                print("g    Enter gdb interactive mode")
                if anchor == "allTeamsArePaused":
                    print("t    Print time")
                    print("s    Step one loop of agents")
                    print("st   Step one loop of agents + time increment")
                    print("e    End stepping")
            elif cmd == "c":
                gdb.execute("continue")
                break
            elif cmd == "q":
                gdb.execute("quit")
                break
            elif cmd == "g":
                break
            elif anchor == "allTeamsArePaused":
                if cmd == "t":
                    gdb.execute("up")
                    gdb.execute("print clock.m_elapsed.count()/1.0e9")
                    gdb.execute("down")
                elif cmd == "s":
                    gdb.execute("up")
                    gdb.execute("call clock.setOneShot()")
                    gdb.execute("continue")
                    break
                elif cmd == "st":
                    gdb.execute("up")
                    gdb.execute("call clock.addTime(100)")
                    gdb.execute("call clock.setOneShot()")
                    gdb.execute("continue")
                    break
                elif cmd == "e":
                    gdb.execute("up")
                    gdb.execute("call clock.setNormalRun()")
                    gdb.execute("continue")
                    break

def gdbMain():
    gdb.events.stop.connect(breakpointHandler)
    breakpoint = gdb.Breakpoint("ScenarioBase::run")
    breakpoint = gdb.Breakpoint("breakAnchor")
    gdb.execute("run")

def launcherMain():
    args = sys.argv
    selfName = args.pop(0)
    args.insert(0, "./marabunta_engine")
    x = ["gdb", "--command", selfName, "--args"] + args
    subprocess.call(x)

if __name__ == "__main__":
    try:
        import gdb
        gdbMain()
    except:
        launcherMain()

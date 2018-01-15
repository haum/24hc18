#!/usr/bin/env python3

import socket, struct
from math import pi
from time import sleep, time

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.bind(('localhost', 2080))
socket.listen(1)
client, address = socket.accept()
client.send(b"\n")

print('time,type,latitude,longitude,heading,team,objid')

def receivedEnd(t):
    print(f'{t},END of frame,,,,,')

def receivedAnt(t, latitude, longitude, heading, team, objid):
    print(f'{t},ANT,{latitude},{longitude},{heading},{team},{objid}')

def receivedFood(t, latitude, longitude, team, objid):
    print(f'{t},FOOD,{latitude},{longitude},0,{team},{objid}')

def receivedNest(t, latitude, longitude, team, objid):
    print(f'{t},NEST,{latitude},{longitude},0,{team},{objid}')

def receivedPheromone(t, latitude, longitude, team, objid):
    print(f'{t},PHEROMONE,{latitude},{longitude},0,{team},{objid}')

def decodeBlock(block, t):
    data = struct.unpack('IfffII', block) 
    cat = data[0]
    longitude = data[1] * 180 / pi
    latitude = data[2] * 180 / pi
    heading = data[3] * 180 / pi
    team = data[4]
    objid = data[5]
    if cat == 4294967295:
        receivedEnd(t)
        return True
    elif cat == 0:
        receivedAnt(t, latitude, longitude, heading, team, objid)
    elif cat == 1:
        receivedFood(t, latitude, longitude, team, objid)
    elif cat == 2:
        receivedNest(t, latitude, longitude, team, objid)
    elif cat == 3:
        receivedPheromone(t, latitude, longitude, team, objid)

blksz = 6*4
line = b''
t0 = time()
t = 0
while True:
    nvline = client.recv(255)
    if nvline == b'':
        break
    line += nvline
    while len(line) >= blksz:
        block = line[:blksz]
        line = line[blksz:]
        if decodeBlock(block, t):
            sleep(0.1)
            t = time() - t0
            client.sendall(b"\n")

client.close()
socket.close()

#!/bin/python

import sys, re

entity=0
memory = []
pheromone_type = 0
see_food_id = 0
see_food_zone = ""
see_food_distance = 0
see_food_amount = 0
food_see_list = []
ant_stock = 0

for line in sys.stdin:
    line = line.strip()
    if line == "BEGIN ANT":
        entity = 1
    if line == "BEGIN NEST":
        entity = 2
    if line.startswith("MEMORY"):
        line_splited = line.split()
        if len(memory) == 0:
            memory.append(line_splited[1])
            memory.append(line_splited[2])
        else:
            memory[0] = line_splited[1]
            memory[1] = line_splited[2]
    if line.startswith("SEE_FOOD"):
        line_splited = line.split()
        see_food_id = line_splited[1]
        see_food_zone = line_splited[2]
        see_food_distance = line_splited[3]
        see_food_amount = line_splited[4]
        liste = []
        liste.append(see_food_id)
        liste.append(see_food_zone)
        liste.append(see_food_distance)
        liste.append(see_food_amount)
        food_see_list.append(liste)
    if line.startswith("STOCK"):
        line_splited = line.split()
        ant_stock = line_splited[1]
    if line == "END":
        if entity == 1:
            if int(ant_stock) <= 100:
                if int(memory[0]) >= 9:
                    print("PUT_PHEROMONE "+str(pheromone_type))
                    memory[0]=0
                    print("SET_MEMORY 0 "+str(int(memory[1])+1))
                elif see_food_zone != "":
                    dist_close_food = 100
                    id_food = 0
                    probably_near_food_zone = ""
                    food_lvl = 0
                    for food_see in food_see_list:
                        if int(food_see[2]) <= int(dist_close_food):
                            dist_close_food = food_see[2]
                            id_food = food_see[0]
                            food_lvl = food_see[3]
                            probably_near_food_zone = food_see[1]
                    if probably_near_food_zone == "NEAR" and int(id_food) != 0 and int(food_lvl) < 10:
                        print("COLLECT "+id_food+" 10")
                    elif probably_near_food_zone == "FAR" and int(id_food) != 0:
                        print("MOVE_TO "+str(id_food))
                elif see_food_zone == "":
                     print("EXPLORE")
                     print("SET_MEMORY "+str(int(memory[0])+1)+" "+str(memory[1]))
        print("END", flush=True)

#!/bin/python

import sys, re

entity=0
memory = []
memory_nest = []
pheromone_type = 0
see_food_id = 0
see_food_zone = ""
see_food_distance = 0
see_food_amount = 0
see_nest_id = 0
see_nest_zone = ""
see_nest_dist = 0
see_nest_friend = 0
ant_stock = 0
ant_stamina = 0
ant_sorted = 0
ant_exist = 0
explo = 0

def log(string):
    with open('out.txt', 'a') as f:
        f.write(str(string)+"\n")

for line in sys.stdin:
    line = line.strip()

    if line == "BEGIN ANT":
        food_see_list = []
        nest_see_list = []
        pheromone_see_list = []
        entity = 1
    if line == "BEGIN NEST":
        entity = 2
    if line.startswith("MEMORY"):
        line_splited = line.split()
        m = []

        m.append(line_splited[1])
        m.append(line_splited[2])

        if entity == 1:
            if len(memory) == 0:
                memory.append(m[0])
                memory.append(m[1])
            else:
                memory[0] = m[0]
                memory[1] = m[1]
        elif entity == 2:
            if len(memory_nest) == 0:
                for x in range(0, 21):
                    if x != 0:
                        memory_nest.append(line_splited[x])
            else:
                for x in range(0, 21):
                    if x != 0:
                        memory_nest[x-1] = line_splited[x]
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
    if line.startswith("SEE_NEST"):
        line_splited = line.split()
        see_nest_id = line_splited[1]
        see_nest_zone = line_splited[2]
        see_nest_dist = line_splited[3]
        see_nest_friend = line_splited[4]
        liste = []
        liste.append(see_nest_id)
        liste.append(see_nest_zone)
        liste.append(see_nest_dist)
        liste.append(see_nest_friend)
        nest_see_list.append(liste)
    if line.startswith("SEE_PHEROMONE"):
        line_splited = line.split()
        see_pheromone_id = line_splited[1]
        see_pheromone_zone = line_splited[2]
        see_pheromone_dist = line_splited[3]
        see_pheromone_type = line_splited[4]
        liste = []
        liste.append(see_pheromone_id)
        liste.append(see_pheromone_zone)
        liste.append(see_pheromone_dist)
        liste.append(see_pheromone_type)
        pheromone_see_list.append(liste)
    if line.startswith("STOCK"):
        line_splited = line.split()
        ant_stock = line_splited[1]
    if line.startswith("STAMINA"):
        line_splited = line.split()
        ant_stamina = line_splited[1]
    if line == "END":
        if entity == 1:
            if int(ant_stock) < 10 and int(memory[1]) < 255:
                if int(memory[0]) >= 8:
                    print("PUT_PHEROMONE "+str(memory[1]))
                    memory[0]=0
                    if int(memory[1]) < 3:
                        memory[1]=str(int(memory[1])+1)
                    else:
                        memory[1] = "1"
                    print("SET_MEMORY 0 "+str(memory[1]))
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
                    if probably_near_food_zone == "NEAR" and int(id_food) != 0 and int(food_lvl) > 10:
                        print("COLLECT "+id_food+" 10")
                    elif probably_near_food_zone == "FAR" and int(id_food) != 0:
                        print("MOVE_TO "+str(id_food))
                elif see_food_zone == "":
                    print("EXPLORE")
                    memory[0]=str(int(memory[0])+1)
                    print("SET_MEMORY "+str(memory[0])+" "+str(memory[1]))
            elif int(memory[1]) >= 255:
                print("SUICIDE")
            else:
                if int(ant_stamina) <= 10:
                    print("EAT")
                else:
                    nest_found_friend = 0
                    nest_friend_zone = "FAR"
                    nest_friend_id = 0
                    for nest_see in nest_see_list:
                        if nest_see[3] == "FRIEND":
                            nest_found_friend = 1
                            nest_friend_zone = nest_see[1]
                            nest_friend_id = nest_see[0]
                    if nest_found_friend == 0:
                        dist_closest_pheromone = 100
                        id_pheromone = 0
                        probably_near_food_zone = ""
                        no_pheromone = 0
                        type_pheromone = ""
                        dist_closest_pheromone_good_type = 100
                        id_pheromone_good_type = 0
                        probably_near_food_zone_good_type = ""
                        no_pheromone_good_type = 0
                        type_pheromone_good_type = 0

                        if int(memory[1]) > 1:
                            m1tmp=int(memory[1])-1
                        else:
                            m1tmp=3


                        log(pheromone_see_list)
                        pheromone_see_good_type_list = []
                        for pheromone_see in pheromone_see_list:
                            if str(pheromone_see[3]) == str(m1tmp):
                                pheromone_see_good_type_list.append(pheromone_see)
                        if pheromone_see_good_type_list != []:
                            near_phero = min(pheromone_see_good_type_list, key=lambda p: p[2])
                            type_pheromone_good_type = near_phero[3]
                            id_pheromone_good_type = near_phero[0]
                            dist_closest_pheromone_good_type = near_phero[2]
                            log(str(id_pheromone_good_type)+": "+str(dist_closest_pheromone_good_type))
                        if pheromone_see_list != []:
                            near_phero = min(pheromone_see_list, key=lambda p: p[2])
                            type_pheromone = near_phero[3]
                            id_pheromone = near_phero[0]
                            dist_closest_pheromone = near_phero[2]
                            log(str(id_pheromone)+": "+str(dist_closest_pheromone))

                        log(str(id_pheromone_good_type)+" != 0"+str(type_pheromone_good_type)+" == "+str(m1tmp))
                        log(str(id_pheromone)+" != 0"+str(id_pheromone_good_type)+" == 0")
                        if str(id_pheromone_good_type) != "0" and str(type_pheromone_good_type) == str(m1tmp):
                            log("good type:"+str(id_pheromone_good_type)+":"+str(dist_closest_pheromone_good_type)+" > 1")
                            if int(dist_closest_pheromone_good_type) > 1:
                                print("MOVE_TO "+id_pheromone_good_type)
                                no_pheromone = 0
                            else:
                                print("SET_MEMORY "+str(memory[0])+" "+str(m1tmp))
                                no_pheromone = 0
                        elif str(id_pheromone) != "0" and str(id_pheromone_good_type) == "0":
                            log(str(id_pheromone)+":"+str(dist_closest_pheromone)+" > 1")
                            if int(dist_closest_pheromone) > 1:
                                print("MOVE_TO "+id_pheromone)
                                no_pheromone = 0
                            else:
                                print("SET_MEMORY "+str(memory[0])+" "+str(m1tmp))
                                no_pheromone = 0
                        else:
                            no_pheromone = 1
                        if no_pheromone == 1:
                            if int(memory[0]) == 10:
                                print("EXPLORE")
                                explo = 1
                            else:
                                print("TURN 180")
                                print("SET_MEMORY 10 "+str(memory[1]))
                                no_pheromone == 0
                    else:
                        if nest_friend_zone == "NEAR":
                            print("NEST "+nest_friend_id)
                            print("SET MEMORY 0 "+str(memory[1]))
                        elif nest_friend_id != 0:
                            print("MOVE_TO "+nest_friend_id)
        elif entity == 2:
            if memory_nest[0] == "0" and memory_nest[1] == "0":
                print("ANT_NEW 0")
                print("SET_MEMORY 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0")
            elif memory_nest[0] == "0" and memory_nest[1] == "1":
                print("ANT_OUT 0 0 0 1")
                print("SET_MEMORY 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0")
        print("END", flush=True)

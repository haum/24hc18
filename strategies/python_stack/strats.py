#! /usr/bin/env python

from math import floor

def log(this):
    with open('/tmp/log', 'a') as fh:
        fh.write(f'{this}\n')

def nest_strat(n):
    """
    MEM:
    0 food_found
    1 num. of explorers
    2 ant_id
    3 num. of gatherer
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    """
    food_found = n.memory[0] == 1
    num_expl = n.memory[1]
    ant_id = n.memory[2]
    num_gatherer = n.memory[3]
    excl = False
    output = []

    if n.ants_in != []:
        for a in n.ants_in:
            if a['type'] == 0 and a['memory'][1] > 0:
                n.memory[0] = 1

    explorer_ants = n.ants.get(0, False)
    gatherer_ants = n.ants.get(1, False)
    if explorer_ants and not food_found:
        output.append(f'ANT_OUT 0 0 {ant_id} 0')
        n.memory[2] += 1
        excl = True
    elif gatherer_ants and food_found:
        output.append(f'ANT_OUT 1 0 {ant_id} 0')
        n.memory[2] += 1
        excl = True
    elif n.stock >= 5 and num_expl < 2:
        n.memory[1] += 1
        output.append('ANT_NEW 0')
    elif food_found and n.stock >= 5 and num_gatherer < 5:
        n.memory[3] += 1
        output.append('ANT_NEW 1')

    memstring = ' '.join(map(str, n.memory))
    return [f'SET_MEMORY {memstring}'] + output


def ant_strat(a):
    """
    MEM
    0 ant id & explored_last
    1 found food & found food last time

    PHEROMONE
    0X Explorers
    10 Route
    """

    excl = False
    output = []
    nests = [_['id'] for _ in a.nests if _['friend'] == 'FRIEND']
    nests_near = [_['id'] for _ in a.nests if _['friend'] == 'FRIEND' and _['zone'] == 'NEAR']
    nest = nests[0] if len(nests) > 0 else False
    nest_near = nests_near[0] if len(nests_near) > 0 else False

    if a.typ == 0:  # explorers
        moved_last = a.memory[0] >= 100
        ant_id = a.memory[0] - 100*floor(a.memory[0]/100)
        found_food_last_time = a.memory[1] >= 200
        found_food = a.memory[1] >= 100
        num_steps = a.memory[1] - 100*floor(a.memory[1]/100)
        current_pheromone = False
        target_pheromone = False

        try:
            self_pheromones = filter(lambda _: _['type'] == ant_id, a.pheromones)
            self_pheromones = [_['id'] for _ in sorted(self_pheromones, key=lambda _: _.get('dist'))]
            target_pheromone = self_pheromones[-1]
            current_pheromone = self_pheromones[0]
        except IndexError:
            pass

        if found_food_last_time:
            output.append('TURN 180')
            excl = True
            a.memory[1] = num_steps + 100
            if moved_last:
                a.memory[0] -= 100
        elif found_food:
            if nest_near:
                output.append(f'NEST {nest_near}')
            elif nest:
                output.append(f'MOVE_TO {nest}')
            elif moved_last and current_pheromone:
                output.append(f'CHANGE_PHEROMONE {current_pheromone} 10')
                excl = True
                a.memory[0] -= floor(a.memory[0]/100)*100
            elif not moved_last and target_pheromone:
                output.append(f'MOVE_TO {target_pheromone}')
                a.memory[0] += 100
            else:
                pass
                # output.append('SUICIDE')
            log(output)
        else:
            if a.food != []:
                output.append('PUT_PHEROMONE 10')
                excl = True
                a.memory[1] = num_steps + 200
            elif num_steps > 4:
                output.append(f'PUT_PHEROMONE {ant_id}')
                excl = True
                a.memory[1] -= 5
            else:
                output.append('EXPLORE')
                a.memory[1] += 1

    elif a.typ == 1:  # gatherer
        has_turned = a.memory[0] == 1
        pheromones = [_['id'] for _ in a.pheromones if _['type'] == 10]
        try:
            target_pheromone = sorted(pheromones, key=lambda _: _.get('dist'))[-1]
        except:
            target_pheromone = False
        food_near = [_['id'] for _ in a.food if _['zone'] == 'NEAR']

        if a.stock == 0:  # not gathered yet
            if a.food != []:
                if food_near != []:
                    output.append(f'COLLECT {food_near[0]} 100')
                else:
                    food_id = sorted(a.food, key=lambda _: _['dist'])[0]
                    output.append(f'MOVE_TO f{food_id}')
            if target_pheromone:
                output.append(f'MOVE_TO f{target_pheromone}')
        else:  # gathered
            if food_near != [] and not has_turned:
                output.append(f'TURN 180')
                a.memory[0] = 1
            elif nest_near:
                output.append(f'NEST {nest_near}')
            elif nest:
                output.append(f'MOVE_TO {nest}')
            else:
                if target_pheromone:
                    output.append(f'MOVE_TO f{target_pheromone}')

    elif a.typ == 2:  # gatherer
        has_turned = a.memory[0] == 1
        pheromones = filter(lambda _: _['type'] == 10, a.pheromones)
        pheromones = [_['id'] for _ in sorted(self_pheromones, key=lambda _: _.get('dist'))]
        try:
            target_pheromone = pheromones[-1]
        except:
            target_pheromone = False
        food_near = [_['id'] for _ in a.food if _['zone'] == 'NEAR']

        if a.stock == 0:  # not gathered yet
            if a.food != []:
                if food_near != []:
                    output.append(f'COLLECT {food_near[0]} 100')
                else:
                    food_id = sorted(a.food, key=lambda _: _['dist'])[0]
                    output.append(f'MOVE_TO f{food_id}')
            if target_pheromone:
                output.append(f'MOVE_TO f{target_pheromone}')
        else:  # gathered
            if food_near != [] and not has_turned:
                output.append(f'TURN 180')
            elif nest_near:
                output.append(f'NEST {nest_near}')
            elif nest:
                output.append(f'MOVE_TO {nest}')
            else:
                if target_pheromone:
                    output.append(f'MOVE_TO f{target_pheromone}')

    memstring = ' '.join(map(str, a.memory))
    return [f'SET_MEMORY {memstring}'] + output

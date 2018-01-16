#! /usr/bin/env python
# -*- coding:utf8 -*-
#
# utils.py
#
# Copyright © 2018 Mathieu Gaborit (matael) <mathieu@matael.org>
#
# Licensed under the "THE BEER-WARE LICENSE" (Revision 42):
# Mathieu (matael) Gaborit wrote this file. As long as you retain this notice
# you can do whatever you want with this stuff. If we meet some day, and you
# think this stuff is worth it, you can buy me a beer or coffee in return


class BaseParser(object):

    def __init__(self, lines):
        self.lines = lines
        self._readers = {}

    def _store(self, cmd, args):
        setattr(self, cmd.lower(), int(args))

    def _process_memory(self, cmd, *memstack):
        self.memory = list(map(int, memstack))

    def read_from_lines(self):

        for l in self.lines:
            command, *args = l.split(' ')
            f = self._readers.get(command)
            if f is not None:
                f(command, *args)
            else:
                pass


class Nest(BaseParser):

    def __init__(self, lines):
        super().__init__(lines)

        self.stock = None
        self.ants = dict()
        self.memory = []
        self.ants_in = []

        self._readers = {
            'STOCK': self._store,
            'MEMORY': self._process_memory,
            'ANT_COUNT': self._process_ant_count,
            'ANT_IN': self._process_ant_in,
        }

        self.read_from_lines()

    def _process_ant_count(self, cmd, typ, count):
        self.ants[int(typ)] = int(count)

    def _process_ant_in(self, cmd, typ, mem1, mem2):
        self.ants_in.append({
            'type': int(typ),
            'memory': [mem1, mem2]
        })


class Ant(BaseParser):

    def __init__(self, lines):
        super().__init__(lines)

        self.typ = None
        self.memory = []
        self.attacked = False
        self.stamina = None
        self.stock = None
        self.pheromones = []
        self.ants = []
        self.nests = []
        self.food = []

        self._readers = {
            'TYPE': self._process_type,
            'STOCK': self._store,
            'STAMINA': self._store,
            'MEMORY': self._process_memory,
            'ATTACKED': self._process_attacked,
            'SEE_PHEROMONE': self._process_see_pheromone,
            'SEE_ANT': self._process_see_ant,
            'SEE_NEST': self._process_see_nest,
            'SEE_FOOD': self._process_see_food,
        }

        self.read_from_lines()

    def _process_type(self, cmd, typ):
        self.typ = int(typ)

    def _process_attacked(self, cmd, *args):
        self.attacked = True

    def _process_see_pheromone(self, cmd, id_, zone, dist, typ, persistance):
        self.pheromones.append({
            'id': int(id_),
            'zone': zone,
            'dist': int(dist),
            'type': int(typ),
            'persistance': int(persistance),
        })

    def _process_see_ant(self, cmd, id_, zone, dist, friend, stamina):
        self.ants.append({
            'id': int(id_),
            'zone': zone,
            'dist': int(dist),
            'friend': friend,
            'stamina': int(stamina),
        })

    def _process_see_nest(self, cmd, id_, zone, dist, friend):
        self.nests.append({
            'id': int(id_),
            'zone': zone,
            'dist': int(dist),
            'friend': friend,
        })

    def _process_see_food(self, cmd, id_, zone, dist, amount):
        self.food.append({
            'id': int(id_),
            'zone': zone,
            'dist': int(dist),
            'amount': int(amount),
        })

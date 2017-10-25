Protocol
********

.. WARNING::
    The cost number is an exemple for the moment.


Generality
==========

The action commands are what you tell to the engine and the infos commands
are the explication of what the engine says to your AI.

The API use stdin and stdout and they are limited to 80 characteres. 

All commands arguments are separated by spaces.

To tell to the engine that the AI has done, write `END` in sdtin.

An entity is able to detect differents elements around it. It got two 
circles of detection. The first, the big one, include the visible elements
arount it. The second, the little one, include elements that are interactive.

Exemple of three identical elements in different relative positions.

::

            [3]

    +-----------------+
    |                 |
    |       [1]       |<---- VISION
    |                 |
    |  +-----------+  |
    |  |           |  |
    |  |    [2]    |<------- INTERECTABLE
    |  |           |  |
    |  +-----------+  |
    |                 |
    +-----------------+

The ``[1]`` is VISIBLE and not INTERACTABLE:

.. code-block:: none

    VISION [ant team=1 hp=3]
    INTERECTABLE []


The ``[2]`` is VISIBLE and INTERACTABLE:

.. code-block:: none

    VISION []
    INTERACTABLE [ant team=1 hp=3]

The ``[3]`` is not VISIBLE and not INTERACTABLE:

.. code-block:: none
    
    VISION []
    INTERACTABLE []

Ant
===

Infos
-----

BEGIN
~~~~~

Returns the information that an entity's turn begin.

.. code-block:: none

    BEGIN <entity>

ENERGY
~~~~~~

Returns entity's amount of energy.

.. code-block:: none

    ENERGY <quantity>

STOCK
~~~~~

Returns entity's food stock.

.. code-block:: none

    STOCK <quantity>

VISION
~~~~~~

Returns, by id, one of the object around an ant. If there is two or more 
objects around, the `VISION` command is repeated as necessary. Id are displayed 
if the number of id > 1.

.. code-block:: none

    VISION <object> <params>

List of objects available with params:

.. code-block:: none

    VISION pheromone <type> <id>

.. code-block:: none

    VISION ant <team_boolean> <energy_level> <id>
    
.. code-block:: none

    VISION food <quantity> <id>

.. code-block:: none

    VISION nest <team_boolean> <id>

INTERACTABLE
~~~~~~~~~~~~

Returns same as `VISION` command but only objects with which that an ant can 
interact.


MEMORY
~~~~~~

Returns an array of two u_int8. Limited to 16 bits.

.. code-block:: none

    MEMORY <u_int8> <u_int8>

STATE
~~~~~

Returns ATTACKED, OCCUPED or NONE.

.. code-block:: none

    STATE <state>


Actions
-------

PUT_PHEROMONE
~~~~~~~~~~~~~

Tell an ant to put a pheromone on the ground, this costs 3 energy.

.. code-block:: none

    PUT_PHEROMONE <type>

CHANGE_PHEROMONE
~~~~~~~~~~~~~~~~

Tell an ant to change a pheromone by id, this costs 2 energy.

.. code-block:: none

    CHANGE_PHEROMONE <id> <type>

RECHARGE_PHEROMONE
~~~~~~~~~~~~~~~~~~

Tell an ant to recharge a pheromone by id. This costs 1 energy.

.. code-block:: none

    RECHARGE_PHEROMONE <id> <type>

EXPLORE
~~~~~~~

Send an ant to explore the world. This costs X energy.

.. code-block:: none

    EXPLORE

ATTACK
~~~~~~

Tell an ant to attack a target by id. This costs X energy.

.. code-block:: none

    ATTACK <target> <id>

MOVE_TO
~~~~~~~

Tell an ant to move in the direction of the target by id. This costs X energy.

.. code-block:: none

    MOVE_TO <target> <id>

COLLECT
~~~~~~~

Tell an ant to collect a quantity of food by id. This costs X energy.

.. code-block:: none

    COLLECT <id> <quantity>

DO_TROPHALLAXIS
~~~~~~~~~~~~~~~

Tell an ant to give a quantity of food to another ant by id. This costs X energy
for the ant who gives food.

.. code-block:: none
    
    DO_TROPHALLAXIS <ant> <quantity>

PUT_DOWN
~~~~~~~~

Tell an ant to put on the ground a quantity of food. This costs X energy.

.. code-block:: none

    PUT_DOWN <quantity>

SAVE
~~~~

Tell an ant to save in u_int8. Limited to 16 bits. This action can be executed 
during the same turn as another action.

.. code-block:: none

    SAVE <u_int8> <u_int8>

SUICIDE
~~~~~~~

Kill the current AI. 

.. code-block:: none

    SUICIDE

NEST
~~~~

Tell an ant to go in the nest. This costs X energy.

.. code-block:: none

    NEST

EAT
~~~

Tell an ant to eat a quantity of food. This gives X energy.

.. code-block:: none

    EAT <quantity>

Nest
====

Infos
-----

BEGIN
~~~~~

Returns the information that an entity's turn begin.

.. code-block:: none

    BEGIN <entity>

ENERGY
~~~~~~

Returns entity's amount of energy.

.. code-block:: none

    ENERGY <quantity>

STOCK
~~~~~

Returns entity's food stock.

.. code-block:: none

    STOCK <quantity>

MEMORY
~~~~~~

Returns an array of 20 u_int8. Limited to 20 octets.

.. code-block:: none

    MEMORY [<u_int8> x20]

ANT_NUMBER
~~~~~~~~~~

Returns the amount of ant in the nest.

.. code-block:: none

    ANT_NUMBER <quantity>

Actions
-------

ANT_OUT
~~~~~~~

Put an ant out. Memory can be set by two u_int8 but it is optional. Default is
0 0. This costs X energy.

.. code-block:: none

    ANT_OUT <u_int8> <u_int8> 

ANT_NEW
~~~~~~~

Create a new ant. This costs X energy.

.. code-block:: none

    ANT_NEW

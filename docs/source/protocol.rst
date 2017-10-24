Protocol
********

.. WARNING::
    The cost number is an exemple for the moment.

The action commands are what you tell to the engine and the infos commands
are the explication of what the engine say to your AI.

Actions
=======

Ant
---

PUT PHEROMONE
~~~~~~~~~~~~~

Tell an ant to put a pheromone on the ground, this costs 3 energy.

.. code-block:: none

    PUT PHEROMONE <type>

CHANGE PHEROMONE
~~~~~~~~~~~~~~~~

Tell an ant to change a pheromone by id, this costs 2 energy.

.. code-block:: none

    CHANGE PHEROMONE <id> <type>

RECHARGE PHEROMONE
~~~~~~~~~~~~~~~~~~

Tell an ant to recharge a pheromone by id. This costs 1 energy.

.. code-block:: none

    RECHARGE PHEROMONE <id> <type>

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

MOVE TO
~~~~~~~

Tell an ant to move in the direction of the target by id. This costs X energy.

.. code-block:: none

    MOVE TO <target> <id>

COLLECT
~~~~~~~

Tell an ant to collect a quantity of food by id. This costs X energy.

.. code-block:: none

    COLLECT <id> <quantity>

DO TROPHALLAXIS
~~~~~~~~~~~~~~~

Tell an ant to give a quantity of food to another ant by id. This costs X energy
for the ant who gives food.

.. code-block:: none
    
    DO TROPHALLAXIS <ant> <quantity>

PUT DOWN
~~~~~~~~

Tell an ant to put on the ground a quantity of food. This costs X energy.

.. code-block:: none

    PUT DOWN <quantity>

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
----

ANT OUT
~~~~~~~

Put an ant out. Memory can be set by two u_int8 but it is optional. Default is
0 0. This costs X energy.

.. code-block:: none

    ANT OUT <u_int8> <u_int8> 

ANT NEW
~~~~~~~

Create a new ant. This costs X energy.

.. code-block:: none

    ANT NEW



Infos
=====

All
---

BEGIN
~~~~~

Return the information that an entity's turn starts.

.. code-block:: none

    BEGIN <entity>

ENERGY
~~~~~~

Return entity's amount of energy.

.. code-block:: none

    ENERGY <quantity>

STOCK
~~~~~

Return entity's food stock.

.. code-block:: none

    STOCK <quantity>


Ant
---

VISION
~~~~~~

Return an array of all objects by id. Id are displayed if the number of id > 1.

.. code-block:: none

    VISION [pheromone type=<type> id=<id>, ant team=<team_boolean> id=<id> eneergy=<energy_level>, food quantity=<quantity> id=<id>, nest team=<team_boolean> id=<id>]

MEMORY
~~~~~~

Return an array of two u_int8. Limited to 16 bits.

.. code-block:: none

    MEMORY <u_int8> <u_int8>

STATE
~~~~~

Return ATTACKED or NONE.

.. code-block:: none

    STATE <state>



INTERACTABLE
~~~~~~~~~~~~

Return an array of all objects interactable like VISION ARRAY.

Nest
----

MEMORY
~~~~~~

Return an array of 20 u_int8. Limited to 20 octets.

.. code-block:: none

    MEMORY [<u_int8> x20]

ANT NUMBER
~~~~~~~~~~

Return the amount of ant in the nest.

.. code-block:: none

    ANT NUMBER <quantity>

Difference between VISION and INTERACTABLE
==========================================

Exemple of tree identical element in different relative position.

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

Protocol
********

Ce document défini l'utilisation du protocol de communication entre "le stratège" et "le similateur".

.. WARNING::
    Les valeurs données dans les exemples le sont à titre indicatif.

Généralités
===========

L'API utilise les ports stdin et stdout et n'accepte que du texte limité à 80 caractères au maximum. 

Chaque message est composé d'une commande et d'un ou plusieurs arguments séparés par des espaces.

Les commandes sont séparées en deux catégories nid et fourmi.

Les commandes sont séparées en deux types "actions" et "infos".

 - Infos: ce sont les données que reçoivent vos entités (nid et fourmi) sur  leur environnemet proche.
 - Actions: ce sont les tâches que vos entités peuvent réaliser pour progresser dans le jeu.

To tell to the engine that the AI has done, write `END` in sdtin.

An entity is able to detect differents elements around it. It got two 
circles of detection. The first, the big one, include the visible elements
arount it. The second, the little one, include elements that are interactive.

Exemple of three identical elements in different relative positions.


.. image:: _static/images/ant.png
   :align: center


= ============== ================================
1 visible et     *VISION []*
  interactif 
                 *INTERACTABLE [ant team=1 hp=3]*
- -------------- --------------------------------
2 visible et non *VISION [ant team=1 hp=3]*
  interactif
                 *INTERACTABLE []*
- -------------- --------------------------------
3 non visible et *VISION []*
  non interactif
                 *INTERACTABLE []*
= ============== ================================


Fourmi
======

Infos (stdout)
--------------

================ ===============================================================
**BEGIN**        | Retourne que l'entité fourmi démarre
                 |
                 | *BEGIN <entity>*                                      
**ENERGY**       | Retourne le niveau d'énérgie d'une fourmi          
                 |
                 | *ENERGY <quantity>*                                   
**STOCK**        | Retourne la quantité de nourriture stockée par une fourmi
                 |
                 | *STOCK <quantity>*
**VISION**       | Retourne les caractéristiques d'un objet présent dans la 
                 | zone de visibilité d'une fourmi (par id). S'il existe 
                 | plusieurs objets, la commande devras être utilisée autant 
                 | de fois que nécessaire. Id are displayed if the number of 
                 | id >1.
                 |
                 | *VISION <object> <params>*
                 |
                 | Voici la liste des objets avec leurs paramètres:
                 |
                 | - *VISION pheromone <type> <id>*
                 |
                 | - *VISION ant <team_boolean> <energy_level> <id>*
                 |
                 | - *VISION food <quantity> <id>*
                 |
                 | - *VISION nest <team_boolean> <id>*
**INTERACTABLE** | Dans la zone Interactive, identique à le zone de 
                 | visibilité
**MEMORY**       | Retourne le contenu de la mémpoire d'une fourmi sour la 
                 | forme d'un tableau de deux u_int8 (soit 2 octets)
                 | 
                 | *MEMORY <u_int8> <u_int8>*
**STATUS**       | Retourne la situation courante d'une fourmi (ATTACKED, 
                 | OCCUPED or NONE
                 |
                 | *STATE <state>* 
================ ===============================================================

Actions (stdin)
---------------

======================= ========================================== =============
**PUT_PHEROMONE**       | Demande à une fourmi de déposer une      Coup en      
                        | phéromone.                               énergie :   3
                        |                                                       
                        | *PUT_PHEROMONE <type>*                                 
**CHANGE_PHEROMONE**    | Demande à une fourmi de modifier une     Coup en      
                        | phéromone.                               énergie :   2
                        |                                                    
                        | *CHANGE_PHEROMONE <id> <type>*                      
**RECHARGE_PHEROMONE**  | Demande à une fourmi de recharger une    Coup en      
                        | phéromone.                               énergie :   1
                        |                                                    
                        | *RECHARGE_PHEROMONE <id> <type>*                    
**EXPLORE**             | Envoi une fourmi en exploration.         Coup en      
                        |                                          énergie :   ?
                        |                                                    
                        | *EXPLORE*                                           
**ATTACK**              | Demande à une fourmi d'attequer une      Coup en      
                        | cible.                                   énergie :   ?
                        |                                                    
                        | *ATTACK <target> <id>*                              
**MOVE_TO**             | Demande à une fourmi de se diriger vers  Coup en      
                        | un emplacement.                          énergie :   ?
                        |                                                    
                        | *MOVE_TO <target> <id>*                  Coup en
**TURN**                | Demande à une fourmi d'éffectuer une     énergie :   ?
                        | rotation sur elle même de x° (de -180° 
                        | à 180°).                                
                        |
                        | *TURN <X>*
**COLLECT**             | Demande à une fourmi de collecter une    Coup en
                        | quantité de nourriture (by id).          énergie :   ?
                        |
                        | *COLLECT <id> <quantity>*
**DO_TROPHALLAXIS**     | Demande à une fourmi de donner de la     Coup en 
                        | nourriture à une autre (by id).          énergie :   ?
                        |
                        | *DO_TROPHALLAXIS <ant> <quantity>*
**PUT_DOWN**            | Demande à une fourmi de déposer une      Coup en
                        | quantité de nourriture au sol.           énergie :   ?
                        |
                        | *PUT_DOWN <quantity>*                    Coup en
**SAVE**                | Demande à une fourmi de mémoriser des    énergie :   ?
                        | données. 2 octets maxi sous la forme 
                        | de 2 u_int8.
                        | *SAVE <u_int8> <u_int8>*
**SUICIDE**             | Tue la fourmi
                        |
                        | *SUICIDE*
**NEST**                | Demande à une fourmi de retourner au     Coup en
                        | nid.                                     énergie :   ?
                        |
                        | *NEST*
**EAT**                 | Demande à une fourmi de manger une       Coup en
                        | quantité de nouriture.                   énergie :   ?
                        |
                        | *EAT <quantity>*
======================= ========================================== =============


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

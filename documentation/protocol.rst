=========
Protocole
=========

Ce document décrit le protocole de communication entre le programme de
"stratégie" et le "simulateur".

.. WARNING::
    Les valeurs données dans les exemples le sont à titre indicatif.

Généralités
===========

Communication
-------------

Par commodité pour le programme de stratégie, la communication s'effectue à
travers ses propres flux d'entrée et de sortie standards (`stdin`, `stdout`).
Il s'agit d'un protocole au format textuel.

Les données sont transmises ligne par ligne (délimitées par un caractère
LF=0x0A=10) avec un maximum de 80 caractères par ligne (saut de ligne inclus).
Chaque ligne contient une seule commande avec des arguments séparés par des
espaces.

Il y a deux types d'entités dont votre programme doit définir les actions selon
votre stratégie : chaque fourmi et la fourmillière.

Ci-après, nous faisons la distinction entre les `informations` que l'entité
perçoit à propos de son environnemet proche et les `actions` qui sont les
opérations qu'elle peut réaliser.

En pratique, le programme de stratégie reçoit d'abord un marqueur de début de
dialogue (`BEGIN ANT` dans le cas d'une fourmi, `BEGIN NEST` dans le cas de la
fourmillière). Suivent ensuite l'ensemble des `informations` perçues puis la
commande `END`.

C'est ensuite à votre programme d'indiquer les actions à réaliser. Attention,
certaines actions ne sont pas cumulables. Votre programme termine la liste des
actions réalisées avec le marqueur `END` et se met en écoute d'un éventuel
marqueur de début pour une nouvelle entité.

Perception de l'environnement
-----------------------------

Les fourmis n'étant pas omniscientes, elles ont une perception limitée à leur
environnement proche. Nous distinguons les éléments à portée avec lesquels il
est possible d'interagir, et les éléments qui sont seulement dans le champ
visuel. Au delà, la fourmi ignore ce qu'il se passe.

.. image:: _static/images/ant.png
   :align: center


= ============== ================================
1 visible et non *VISION [ant team=1 hp=3]*
  interactif
                 *INTERACTABLE []*
- -------------- --------------------------------
2 visible et     *VISION []*
  interactif
                 *INTERACTABLE ant team=1 hp=3[]*
- -------------- --------------------------------
3 non visible et *VISION []*
  non interactif
                 *INTERACTABLE []*
= ============== ================================


Fourmi
======

Informations
------------

================ ===============================================================
**BEGIN ANT**    | Informe du début de la communication
**ENERGY <qté>** | Informe du niveau d'énergie (`qté`) de la fourmi
**STOCK <qté>**  | Informe de la quantité de nourriture stockée par la fourmi
**VISION**       | Retourne les caractéristiques d'un objet présent dans la
                 | zone de visibilité d'une fourmi (par id). S'il existe
                 | plusieurs objets, la commande devra être utilisée autant
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
**INTERACTABLE** | Dans la zone Interactive, identique à la zone de
                 | visibilité
**MEMORY**       | Retourne le contenu de la mémoire d'une fourmi sous la
                 | forme d'un tableau de deux u_int8 (soit 2 octets)
                 |
                 | *MEMORY <u_int8> <u_int8>*
**STATUS**       | Retourne la situation courante d'une fourmi (ATTACKED,
                 | OCCUPED or NONE)
                 |
                 | *STATE <state>*
================ ===============================================================

Actions
-------

======================= ========================================== =============
Commande                Description                                Coût
                                                                   énergétique
======================= ========================================== =============
**PUT_PHEROMONE**       | Demande à une fourmi de déposer une
                        | phéromone.                               3
----------------------- ------------------------------------------ -------------
                        | *PUT_PHEROMONE <type>*
**CHANGE_PHEROMONE**    | Demande à une fourmi de modifier une
                        | phéromone.                               2
----------------------- ------------------------------------------ -------------
                        | *CHANGE_PHEROMONE <id> <type>*
**RECHARGE_PHEROMONE**  | Demande à une fourmi de recharger une
                        | phéromone.                               1
                        |
                        | *RECHARGE_PHEROMONE <id> <type>*
**EXPLORE**             | Envoi une fourmi en exploration.
                        |                                          ?
                        |
                        | *EXPLORE*
**ATTACK**              | Demande à une fourmi d'attaquer une
                        | cible.                                   ?
                        |
                        | *ATTACK <target> <id>*
**MOVE_TO**             | Demande à une fourmi de se diriger vers
                        | un emplacement.                          ?
                        |
                        | *MOVE_TO <target> <id>*
**TURN**                | Demande à une fourmi d'effectuer une
                        | rotation sur elle-même de x° (de -180°   ?
                        | à 180°).
                        |
                        | *TURN <X>*
**COLLECT**             | Demande à une fourmi de collecter une
                        | quantité de nourriture (by id).          ?
                        |
                        | *COLLECT <id> <quantity>*
**DO_TROPHALLAXIS**     | Demande à une fourmi de donner de la
                        | nourriture à une autre (by id).          ?
                        |
                        | *DO_TROPHALLAXIS <ant> <quantity>*
**PUT_DOWN**            | Demande à une fourmi de déposer une
                        | quantité de nourriture au sol.           ?
                        |
                        | *PUT_DOWN <quantity>*
**SAVE**                | Demande à une fourmi de mémoriser des
                        | données. 2 octets maxi sous la forme     ?
                        | de 2 u_int8.
                        | *SAVE <u_int8> <u_int8>*
**SUICIDE**             | Tue la fourmi
                        |
                        | *SUICIDE*
**NEST**                | Demande à une fourmi de retourner au
                        | nid.                                     ?
                        |
                        | *NEST*
**EAT**                 | Demande à une fourmi de manger une
                        | quantité de nourriture.                   ?
                        |
                        | *EAT <quantity>*
======================= ========================================== =============


Fourmillière
============

Informations
------------

================ ===============================================================
**BEGIN**        | Retourne que l'entité nid démarre
                 |
                 | *BEGIN <entity>*
**ENERGY**       | Retourne le niveau d'énergie du nid
                 |
                 | *ENERGY <quantity>*
**STOCK**        | Retourne la quantité de nourriture stockée par le nid
                 |
                 | *STOCK <quantity>*
**MEMORY**       | Retourne le contenu de la mémoire du nid sous la forme d'un
                 | tableau de 20 u_int8 (soit 20 octets)
                 |
                 | *MEMORY [<u_int8> x20]*
**ANT_NUMBER**   | Retourne le nombre de fourmi à l'intérieur du nid
                 |
                 | *ANT_NUMBER <quantity>*
================ ===============================================================

Actions
-------

======================= ========================================== =============
Commande                Description                                Coût
                                                                   énergétique
======================= ========================================== =============
**ANT_OUT**             | Sort une fourmi du nid. Les deux octets
                        | de mémoire de la fourmi peuvent êtres    ?
                        | initialisés à ce moment. Par défaut, ces
                        | deux octets sont initialisés à zéro.
                        |
                        | *ANT_OUT <u_int8> <u_int8>*
**ANT_NEW**             | Crée une nouvelle fourmi.
                        |                                          ?
                        |
                        | *ANT_NEW*
======================= ========================================== =============

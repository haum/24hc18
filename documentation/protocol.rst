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
votre stratégie : chaque fourmi et la fourmilière.

Ci-après, nous faisons la distinction entre les `informations` que l'entité
perçoit à propos de son environnement proche et les `actions` qui sont les
opérations qu'elle peut réaliser.

En pratique, le programme de stratégie reçoit d'abord un marqueur de début de
dialogue (`BEGIN ANT` dans le cas d'une fourmi, `BEGIN NEST` dans le cas de la
fourmilière). Suivent ensuite l'ensemble des `informations` perçues puis la
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
visuel. Au-delà, la fourmi ignore ce qu'il se passe.

.. image:: _static/images/ant.png
   :align: center


= ============== ================================
1 visible et non | *VISION [ant team=1 hp=3]*
  interactif     | *INTERACTABLE []*
- -------------- --------------------------------
2 visible et     | *VISION []*
  interactif     | *INTERACTABLE [ant team=1 hp=3[]*
- -------------- --------------------------------
3 non visible et | *VISION []*
  non interactif | *INTERACTABLE []*
= ============== ================================


Fourmi
======

Informations
------------

=============================================== =============================================
Commande                                        Description
=============================================== =============================================
*BEGIN ANT*                                     | Informe du début de la communication
----------------------------------------------- ---------------------------------------------
*ENERGY <qté>*                                  | Informe du niveau d'énergie (`qté`) 
                                                | de la fourmi
----------------------------------------------- ---------------------------------------------
*STOCK <qté>*                                   | Informe de la quantité de nourriture 
                                                | stockée par la fourmi
----------------------------------------------- ---------------------------------------------
*VISION pheromon <type> <id>*                   | Informe de la présence de phéromones dans 
                                                | la zone de visibilité.
----------------------------------------------- ---------------------------------------------
*VISION ant <team_boolean> <energy_level> <id>* | Informe de la présence de fourmis dans la 
                                                | zone de visibilité.
----------------------------------------------- ---------------------------------------------
*VISION food <quantity> <id>*                   | Informe de la présence de nourriture dans 
                                                | la zone de visibilité.
----------------------------------------------- ---------------------------------------------
*VISION nest <team_boolean> <id>*               | Informe de la présence de fourmilières 
                                                | dans la zone de visibilité.
----------------------------------------------- ---------------------------------------------
*INTERACTABLE*                                  | Fourni les mêmes informations que VISION,
                                                | mais pour la zone interactive.
----------------------------------------------- ---------------------------------------------
*MEMORY <u_int8> <u_int8>*                      | Informe du contenu de la mémoire d'une 
                                                | fourmi sous la forme d'un tableau de 
                                                | deux u_int8 (soit 2 octets)
----------------------------------------------- ---------------------------------------------
*STATUS <state>*                                | Informe sur l'etat de situation courante 
                                                | d'une fourmi (ATTACKED, OCCUPED 
                                                | où NONE).
=============================================== =============================================

Actions
-------

================================== ========================================== ========
Commande                           Description                                Coût
================================== ========================================== ========
*PUT_PHEROMONE <type>*             | Demande à une fourmi de déposer 
                                   | une phéromone.                           3
---------------------------------- ------------------------------------------ --------
*CHANGE_PHEROMONE <id> <type>*     | Demande à une fourmi de modifier 
                                   | une phéromone.                           2
---------------------------------- ------------------------------------------ --------
*RECHARGE_PHEROMONE <id> <type>*   | Demande à une fourmi de recharger une
                                   | phéromone.                               1
---------------------------------- ------------------------------------------ --------
*EXPLORE*                          | Demande à une fourmi de ce déplacer 
                                   | déplacer 
---------------------------------- ------------------------------------------ --------
*ATTACK <target> <id>*             | Demande à une fourmi d'attaquer une
                                   | cible.                                   ?
---------------------------------- ------------------------------------------ --------
*MOVE_TO <target> <id>*            | Demande à une fourmi de se diriger vers
                                   | un emplacement.                          ?
---------------------------------- ------------------------------------------ --------
*TURN <X>*                         | Demande à une fourmi d'effectuer une
                                   | rotation sur elle-même de x° (de -180°   ?
                                   | à 180°).
---------------------------------- ------------------------------------------ --------
*COLLECT <id> <quantity>*          | Demande à une fourmi de collecter une
                                   | quantité de nourriture (by id).          ?
---------------------------------- ------------------------------------------ --------
*DO_TROPHALLAXIS <ant> <quantity>* | Demande à une fourmi de donner de la
                                   | nourriture à une autre (by id).          ?
---------------------------------- ------------------------------------------ --------
*DROP <quantity>*                  | Demande à une fourmi de déposer une
                                   | quantité de nourriture au sol.           ?
---------------------------------- ------------------------------------------ --------
*SAVE <u_int8> <u_int8>*           | Demande à une fourmi de mémoriser des
                                   | données. 2 octets maxi sous la forme     ?
                                   | de 2 u_int8.
---------------------------------- ------------------------------------------ --------
*SUICIDE*                          | Tue la fourmi
---------------------------------- ------------------------------------------ --------
*NEST*                             | Demande à une fourmi de retourner au
                                   | nid.                                     ?
---------------------------------- ------------------------------------------ --------
*EAT <quantity>*                   | Demande à une fourmi de manger une
                                   | quantité de nourriture.                   ?
================================== ========================================== ========


Fourmilière
============

Informations
------------

========================== =====================================================
Commande                   Description
========================== =====================================================
*BEGIN NEST*               | Informe du début de la communication
-------------------------- -----------------------------------------------------
*ENERGY <quantity>*        | Informe du niveau d'énergie (qté) de la
                           | fourmilière
-------------------------- -----------------------------------------------------
*STOCK <quantity>*         | Informe de la quantité de nourriture stockée  
                           | dans le fourmilière
-------------------------- ----------------------------------------------------- 
*MEMORY [<u_int8> x20]*    | Informe du contenu de la mémoire de la
                           | fourmilière sous la forme d'un tableau de 
                           | 20 u_int8 (soit 20 octets)
-------------------------- -----------------------------------------------------
*ANT_NUMBER <quantity>*    | Informe du nombre de fourmis à l'intérieur 
                           | de la fourmilière
========================== =====================================================

Actions
-------

============================ ========================================== ========
Commande                     Description                                Coût
============================ ========================================== ========
*ANT_OUT <u_int8> <u_int8>*  | Sort une fourmi du nid. Les deux octets
                             | de mémoire de la fourmi peuvent êtres    ?
                             | initialisés à ce moment. Par défaut, ces
                             | deux octets sont initialisés à zéro.
---------------------------- ------------------------------------------ --------
*ANT_NEW*                    | Crée une nouvelle fourmi.
============================ ========================================== ========

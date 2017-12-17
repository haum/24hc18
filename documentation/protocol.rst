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

`BEGIN ANT`
  Informe du début de la communication. Cette commande est toujours envoyée en
  premier.

`TYPE <type>`
  Type de fourmi. Ce type est défini par la fourmillière quand elle crée cette
  fourmi.

  ======  ======
  `type`  Nombre compris entre 0 et 255 inclus.
  ======  ======

`MEMORY <m0> <m1>`
  Indique le contenu de la mémoire de la fourmi.

  ====  ======
  `m0`  Nombre compris entre 0 et 255 inclus.
  `m1`  Nombre compris entre 0 et 255 inclus.
  ====  ======

`SEE_PHEROMONE <id> <zone> <type>`
  Indique la présence d'une phéromone à proximité.

  ======  ======
  `id`    Nombre temporaire utilisé pour faire référence à cet objet dans les
          actions.
  ------  ------
  `zone`  Zone d'interaction.

          Prend les valeurs `FAR` ou `NEAR`.
  ------  ------
  `type`  Type de phéromone détectée.

          Ce type est défini par la fourmi ayant déposé la phéromone.

          Les phéromones des équipes adverses ne sont pas listées.
  ======  ======

`SEE_ANT <id> <zone> <friend> <life>`
  Indique la présence d'une fourmi à proximité.

  ========  ======
  `id`      Nombre temporaire utilisé pour faire référence à cet objet dans les
            actions.
  --------  ------
  `zone`    Zone d'interaction.

            Prend les valeurs `FAR` ou `NEAR`.
  --------  ------
  `friend`  Indique si la fourmi est-elle de notre fourmillière.

            Prend les valeurs `FRIEND` ou `ENEMY`.
  --------  ------
  `life`    Indique le niveau de vie de la fourmi.

            Nombre indiquant la vie restante. (À remplacer par des seuils ?)
  ========  ======

`SEE_NEST <id> <zone> <friend>`
  Indique la présence d'une fourmillière à proximité.

  ========  ======
  `id`      Nombre temporaire utilisé pour faire référence à cet objet dans les
            actions.
  --------  ------
  `zone`    Zone d'interaction.

            Prend les valeurs `FAR` ou `NEAR`.
  --------  ------
  `friend`  Indique si la fourmi est-elle de notre fourmillière.

            Prend les valeurs `FRIEND` ou `ENEMY`.
  ========  ======

`SEE_FOOD <id> <zone> <amount>`
  Indique la présence d'une fourmillière à proximité.

  ========  ======
  `id`      Nombre temporaire utilisé pour faire référence à cet objet dans les
            actions.
  --------  ------
  `zone`    Zone d'interaction.

            Prend les valeurs `FAR` ou `NEAR`.
  --------  ------
  `amount`  Indique le niveau de nourriture.

            Nombre indiquant la quantité disponible restante.
  ========  ======


Non implémenté :

======================================== ======================================
Commande                                 Description
======================================== ======================================
*ENERGY <qté>*                           | Informe du niveau d'énergie (`qté`)
                                         | de la fourmi
---------------------------------------- --------------------------------------
*STOCK <qté>*                            | Informe de la quantité de nourriture
                                         | stockée par la fourmi
---------------------------------------- --------------------------------------
*STATUS <state>*                         | Informe sur l'etat de situation
                                         | courante d'une fourmi (ATTACKED,
                                         | OCCUPED ou NONE).
======================================== ======================================

Actions
-------

================================== =================================== ========
Commande                           Description                         Coût
================================== =================================== ========
*PUT_PHEROMONE <type>*             | Demande à une fourmi de déposer 
                                   | une phéromone.                    3
---------------------------------- ----------------------------------- --------
*CHANGE_PHEROMONE <id> <type>*     | Demande à une fourmi de modifier 
                                   | une phéromone.                    2
---------------------------------- ----------------------------------- --------
*RECHARGE_PHEROMONE <id>*          | Demande à une fourmi de recharger
                                   | une phéromone.                    1
---------------------------------- ----------------------------------- --------
*EXPLORE*                          | Demande à une fourmi de ce
                                   | déplacer                          1
---------------------------------- ----------------------------------- --------
*ATTACK <id>*                      | Demande à une fourmi d'attaquer
                                   | une cible.
---------------------------------- ----------------------------------- --------
*MOVE_TO <id>*                     | Demande à une fourmi de se
                                   | diriger vers un emplacement.      2
---------------------------------- ----------------------------------- --------
*TURN <X>*                         | Demande à une fourmi d'effectuer
                                   | une rotation sur elle-même de x°  1
                                   | (de -180° à 180°).
---------------------------------- ----------------------------------- --------
*COLLECT <id> <quantity>*          | Demande à une fourmi de collecter
                                   | une quantité de nourriture (by    4
                                   | id).
---------------------------------- ----------------------------------- --------
*DO_TROPHALLAXIS <ant> <quantity>* | Demande à une fourmi de donner de
                                   | la nourriture à une autre (by
                                   | id).
---------------------------------- ----------------------------------- --------
*DROP <quantity>*                  | Demande à une fourmi de déposer
                                   | une quantité de nourriture au     4
                                   | sol.
---------------------------------- ----------------------------------- --------
*SAVE <u_int8> <u_int8>*           | Demande à une fourmi de mémoriser
                                   | des données. 2 octets maxi sous   0
                                   | la forme de 2 u_int8.
---------------------------------- ----------------------------------- --------
*SUICIDE*                          | Tue la fourmi                     0
---------------------------------- ----------------------------------- --------
*NEST*                             | Demande à une fourmi de retourner
                                   | au nid. Cette action n'est        2
                                   | possible que lorsque le nid est
                                   | dans la zone interactive.
---------------------------------- ----------------------------------- --------
*EAT <quantity>*                   | Demande à une fourmi de manger
                                   | une quantité de nourriture.       0
================================== =================================== ========


Fourmilière
============

La fourmilère abrite le stock de ressource de la colonie ainsi q'une partie de
sa population.

La population vivant dans la fourmilère consome ses ressources. Par conséquent,
le stock de ressources baisse proportionellement à la population selon le
calcul suivant : 1 point de stock toutes les 100ms et par palier de 100
fourmis.

Voici un exemple:

============== ================================================================
Population     Coût en ressources
-------------- ----------------------------------------------------------------
85 fourmis     1 point de stosck / 100ms
-------------- ----------------------------------------------------------------
512 fourmis    6 points de stock / 100ms                                       
============== ================================================================

Le stock et la population de la fourmilière ne sont pas limités.

Lorsque la population où le stock de la fourmilière tombent à 0, cette dernière
n'est pas détriute. Elle reste, désertée sur le terrain et peut être
recolonisée par son équipe uniquement.

Informations
------------

========================== ====================================================
Commande                   Description
========================== ====================================================
*BEGIN NEST*               | Informe du début de la communication
-------------------------- ----------------------------------------------------
*ENERGY <quantity>*        | Informe du niveau d'énergie (qté) de la
                           | fourmilière
-------------------------- ----------------------------------------------------
*STOCK <quantity>*         | Informe de la quantité de nourriture stockée  
                           | dans le fourmilière
-------------------------- ----------------------------------------------------
*MEMORY [<u_int8> x20]*    | Informe du contenu de la mémoire de la
                           | fourmilière sous la forme d'un tableau de 
                           | 20 u_int8 (soit 20 octets)
-------------------------- ----------------------------------------------------
*ANT_NUMBER <quantity>*    | Informe du nombre de fourmis à l'intérieur 
                           | de la fourmilière
========================== ====================================================

Actions
-------

============================ ========================================== =======
Commande                     Description                                Coût
============================ ========================================== =======
*ANT_OUT <u_int8> <u_int8>*  | Sort une fourmi du nid. Les deux octets
                             | de mémoire de la fourmi peuvent êtres    1
                             | initialisés à ce moment. Par défaut, ces
                             | deux octets sont initialisés à zéro.
---------------------------- ------------------------------------------ -------
*ANT_NEW*                    | Crée une nouvelle fourmi.                5
                             | Chaque fourmi née avec le maximum de 
                             | ses points de stmina.
============================ ========================================== =======

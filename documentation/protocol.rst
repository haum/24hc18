Protocole
*********

Ce document défini l'utilisation du protocole de communication entre 
"le stratège" et "le simulateur".

.. WARNING::
    Les valeurs données dans les exemples le sont à titre indicatif.

Généralités
===========

L'API utilise les ports stdin et stdout et n'accepte que du texte limité à 80 
caractères au maximum. 

Chaque message est composé d'une commande et d'un ou plusieurs arguments séparés
par des espaces.

Les commandes sont séparées en deux catégories nid et fourmi.

Les commandes sont séparées en deux types "actions" et "infos".

 - Infos: ce sont les données que reçoivent vos entités (nid et fourmi) sur  
   leur environnement proche.
 - Actions: ce sont les tâches que vos entités peuvent réaliser pour progresser 
   dans le jeu.

Pour signifier au simulateur qu'une fourmi ou un nid a effectué toutes ces 
actions, envoyez *END* sur stdin.

La capacité d'une fourmi à percevoir son environnement selon la distance est 
symbolisé par deux cercles de détection.

Ci-dessous, une représentation des cercles de détection.


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

Infos (stdout)
--------------

================ ===============================================================
**BEGIN**        | Retourne que l'entité fourmi démarre
                 |
                 | *BEGIN <entity>*                                      
**ENERGY**       | Retourne le niveau d'énergie d'une fourmi          
                 |
                 | *ENERGY <quantity>*                                   
**STOCK**        | Retourne la quantité de nourriture stockée par une fourmi
                 |
                 | *STOCK <quantity>*
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

Actions (stdin)
---------------

======================= ========================================== =============
**PUT_PHEROMONE**       | Demande à une fourmi de déposer une      Coût en      
                        | phéromone.                               énergie :   3
                        |                                                       
                        | *PUT_PHEROMONE <type>*                                 
**CHANGE_PHEROMONE**    | Demande à une fourmi de modifier une     Coût en      
                        | phéromone.                               énergie :   2
                        |                                                    
                        | *CHANGE_PHEROMONE <id> <type>*                      
**RECHARGE_PHEROMONE**  | Demande à une fourmi de recharger une    Coût en      
                        | phéromone.                               énergie :   1
                        |                                                    
                        | *RECHARGE_PHEROMONE <id> <type>*                    
**EXPLORE**             | Envoi une fourmi en exploration.         Coût en      
                        |                                          énergie :   ?
                        |                                                    
                        | *EXPLORE*                                           
**ATTACK**              | Demande à une fourmi d'attaquer une      Coût en      
                        | cible.                                   énergie :   ?
                        |                                                    
                        | *ATTACK <target> <id>*                              
**MOVE_TO**             | Demande à une fourmi de se diriger vers  Coût en      
                        | un emplacement.                          énergie :   ?
                        |                                                    
                        | *MOVE_TO <target> <id>*                 
**TURN**                | Demande à une fourmi d'effectuer une     Coût en
                        | rotation sur elle-même de x° (de -180°   énergie :   ?
                        | à 180°).                                
                        |
                        | *TURN <X>*
**COLLECT**             | Demande à une fourmi de collecter une    Coût en
                        | quantité de nourriture (by id).          énergie :   ?
                        |
                        | *COLLECT <id> <quantity>*
**DO_TROPHALLAXIS**     | Demande à une fourmi de donner de la     Coût en 
                        | nourriture à une autre (by id).          énergie :   ?
                        |
                        | *DO_TROPHALLAXIS <ant> <quantity>*
**PUT_DOWN**            | Demande à une fourmi de déposer une      Coût en
                        | quantité de nourriture au sol.           énergie :   ?
                        |
                        | *PUT_DOWN <quantity>*                   
**SAVE**                | Demande à une fourmi de mémoriser des    Coût en
                        | données. 2 octets maxi sous la forme     énergie :   ?
                        | de 2 u_int8.
                        | *SAVE <u_int8> <u_int8>*
**SUICIDE**             | Tue la fourmi
                        |
                        | *SUICIDE*
**NEST**                | Demande à une fourmi de retourner au     Coût en
                        | nid.                                     énergie :   ?
                        |
                        | *NEST*
**EAT**                 | Demande à une fourmi de manger une       Coût en
                        | quantité de nourriture.                   énergie :   ?
                        |
                        | *EAT <quantity>*
======================= ========================================== =============


Nid
===

Infos (stdout)
--------------

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

Actions (stdin)
---------------

======================= ========================================== =============
**ANT_OUT**             | Sort une fourmi du nid. Les deux octets  Coût en 
                        | de mémoire de la fourmi peuvent êtres    énergie :   ?
                        | initialisés à ce moment. Par défaut, ces
                        | deux octets sont initialisés à zéro.
                        |
                        | *ANT_OUT <u_int8> <u_int8>*
**ANT_NEW**             | Crée une nouvelle fourmi.                Coût en
                        |                                          énergie :   ?
                        |
                        | *ANT_NEW*
======================= ========================================== =============

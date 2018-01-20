========
Scénario
========

Ce chapitre indique la syntaxe des fichiers de scénario. Il s'agit des
descriptions de l'état initial des simulations.

Le scénario à jouer est passé en ligne de commande du simulateur avec l'option
`-s`. Chaque ligne du fichier est interprétée pour construire la situation
initiale, puis le jeu commence.

Une partie des commandes servent uniquement à initialiser une situation de test
et ne seront donc pas utilisées en match.

Commandes
=========

`MAXTEAMS <teams>`
  Définis le nombre maximal d'équipes simultanées dans la simulation.

  Il est souhaitable de définir la position des fourmilières de chaque équipe
  potentielle dans la suite du fichier.

  =======  ======
  `teams`  Nombre maximal d'équipes acceptées
  =======  ======

`DURATION <s>`
  Définis la durée de la simulation.

  ===  ======
  `s`  Temps de simulation en secondes
  ===  ======

`NEST_POSITION <team> <latitude> <longitude>`
  Définis la position initiale d'une fourmilière.

  Si la commande est répétée pour une même équipe, la position est écrasée. Il
  est préférable d'avoir au moins une commande de ce type par équipe acceptée
  dans la simulation.

  ===========  ======
  `team`       Numéro de l'équipe à qui appartient la fourmilière (débute à 0).
  `latitude`   Latitude de l'objet en degrés (-90 à 90).
  `longitude`  Longitude de l'objet en degrés (-180 à 180).
  ===========  ======

`NEST_POPULATION <team> <type> <nb>`
  Définis la population dans une fourmilière.

  Chaque type de chaque équipe comportant au moins une fourmi initiale doit
  avoir sa propre ligne dans le fichier de scénario.

  ======  ======
  `team`  Numéro de l'équipe à qui appartient la fourmilière (débute à 0).
  `type`  Type de fourmis. Nombre compris entre 0 et 255 inclus.
  `nb`    Nombre de fourmis de ce type.
  ======  ======

`NEST_FOOD <team> <amount>`
  Définis la quantité de nourriture initialement dans la fourmilière.

  ========  ======
  `team`    Numéro de l'équipe à qui appartient la fourmilière (débute à 0).
  `amount`  Quantité de nourriture initiale.
  ========  ======

`ANT <team> <type> <latitude> <longitude> <heading> <stamina> <memory1> <memory2>`
  Ajoute une fourmi dans la simulation.

  ===========  ======
  `team`       Numéro de l'équipe à laquelle appartient la fourmi (débute à 0).
  `type`       Type de fourmi.
  `latitude`   Latitude de l'objet en degrés (-90 à 90).
  `longitude`  Longitude de l'objet en degrés (-180 à 180).
  `heading`    Cap de l'objet en degrés (-180 à 180 référencés au nord)
  `stamina`    Stamina initiale.
  `memory1`    Mémoire 1 initiale.
  `memory2`    Mémoire 2 initiale.
  ===========  ======

`PHEROMONE <team> <type> <latitude> <longitude>`
  Ajoute une phéromone dans la simulation.

  ===========  ======
  `team`       Numéro de l'équipe à laquelle appartient la phéromone (débute à 0).
  `type`       Type de phéromone.
  `latitude`   Latitude de l'objet en degrés (-90 à 90).
  `longitude`  Longitude de l'objet en degrés (-180 à 180).
  ===========  ======

`FOOD <latitude> <longitude> <initialValue> <chargeRate> <chargeMax> <totalChargeMax> <deadIfEmpty>`
  Ajoute une source de nourriture dans la simulation.

  ================  ======
  `latitude`        Latitude de l'objet en degrés (-90 à 90).
  `longitude`       Longitude de l'objet en degrés (-180 à 180).
  `initialValue`    Quantité initiale dans la source.
  `chargeRate`      Taux de rechargement en quantité par 100ms (peut être
                    décimal, par exemple 0.1).
  `chargeMax`       Quantité maximale.
  `totalChargeMax`  Quantité maximale ajoutée pendant tout le match.
  `deadIfEmpty`     La source doit-elle se tarir (1) ou continuer à se recharger
                    (0) si elle est complètement consommée.
  ================  ======

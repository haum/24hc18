===================
Scénarii des matchs
===================

Match 1 -- Nourriture autour du nid
===================================

Le fichier de scenario est `disponible ici`_ ou directement copiable depuis l'exemple
ci-dessous::

  MAXTEAMS 1
  DURATION 60
  NEST_POSITION 0 0 0
  NEST_FOOD 0 200

  FOOD 4 0 200 400 400 999999 0
  FOOD 3 1 200 400 400 999999 0
  FOOD 2 3 200 400 400 999999 0
  FOOD 0 4 200 400 400 999999 0
  FOOD -1 3 200 400 400 999999 0
  FOOD -3 1 200 400 400 999999 0
  FOOD -4 0 200 400 400 999999 0
  FOOD -3 -2 200 400 400 999999 0
  FOOD -2 -3 200 400 400 999999 0
  FOOD 0 -4 200 400 400 999999 0
  FOOD 2 -3 200 400 400 999999 0
  FOOD 3 -2 200 400 400 999999 0

Match 2 -- On prend les mêmes et on recommence !
================================================

Ce scénario est le même que celui proposé pour le match 1.

Match 3 -- La nourriture devient périssable
===========================================

Le scénario pour le match 3 est le même que le précédent, mais cette fois ci, la nourriture 
est périssable. Si la quantité atteint 0, la source de nourriture sera détruite. Également, les 
sources de nourriture se régénèrent moins vite (25/100ms).

Match 4 -- Deux nids et de la nourriture autour
===============================================

De la nourriture (avec les mêmes caractéristiques que le match précédent) est disposée en
cercle autour de deux nids adverses.

.. _disponible ici: _static/scenarii/01_food_around_nest_1t_fnd_60s.cfg

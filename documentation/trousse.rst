=================
Trousse de survie
=================

Langages de programmation
=========================

Le simulateur peut interagir avec tout programme exécutable sur le système mais
le serveur de jeu ne supporte pas tous les langages imaginables. Nous avons
limité notre support aux langages suivants :

 - AsciiDots
 - C
 - C++
 - Java
 - Perl
 - Python 2 [#f1]_
 - Python 3 [#f1]_

Vous êtes libres d'utiliser le langage qui vous convient parmi cette liste.

Flush
=====

Le dialogue entre le programme de stratégie et le simulateur se fait à travers
des flux de données qui restent ouverts. Or les flux de données sont souvent
bufferisés pour augmenter l'efficacité des transferts en minimisant les
changements de contexte. Dans notre cas, ce mécanisme est également utile, mais
il faut veiller à bien envoyer les données aux moments opportuns.

Pour cela, nous vous encourageons à trouver la commande qui permet de réaliser
un flush (c'est le terme souvent utilisé dans les API) du buffer, c'est-à-dire
qui envoie effectivement le contenu des mémoires tampon dans le flux.

Vous pourrez donc le faire juste après avoir envoyé la commande `END` et ainsi
être certains que le simulateur n'attendra pas indéfiniment des commandes qui
seraient coincées dans un buffer intermédiaire.

Suivi de versions
=================

Pour simplifier le développement logiciel en équipe et son suivi, il est
fréquent d'utiliser un gestionnaire de version. Dans le cas de marabunta, nous
vous demandons d'utiliser l'outil `git`.

Il s'agit d'un gestionnaire très utilisé sur lequel vous trouverez facilement de
la documentation en ligne dans toutes les langues.  Notons par exemple la
`documentation officielle <https://git-scm.com/>`_ en anglais.

.. NOTE::
   Au delà du sujet, nous vous conseillons fortement d'apprendre à maîtriser ce
   genre d'outil. C'est un atout précieux pour produire du code de qualité et
   maîtriser le développement au cours du temps, seul ou en équipe. [#f2]_

Pour chaque match, votre code est compilé et exécuté sur le serveur de jeu. Pour
simplifier la récupération de votre travail, ce serveur tire profit de l'outil
`git` en rapatriant les sources de votre stratégie depuis son dépôt.

Vous avez le choix de l'hébergement dès lors que le dépôt est accessible
publiquement ou que la machine virtuelle est autorisée à le récupérer. Si vous
n'avez pas de serveur personnel à disposition, vous pouvez utiliser des
services tels que `Framagit <https://framagit.org>`_ ou encore `Github
<https://github.com>`_.

.. NOTE::
   Sur Framagit, donner les droits à l'utilisateur "marabunta" satisfait cette
   condition.

Ressources
==========

Vous avez accès à la présente documentation qui contient notamment la
description du protocole de communication entre le programme de stratégie et le
simulateur.

Vous disposez également du code source de notre simulateur, libre à vous de le
compiler sur vos pc pour tester vos stratégies.


Environnement clos
==================

Lors des matchs sur le serveur de jeu, les stratégies seront isolées les unes
des autres dans des environnements clos (jails). Ces environnements restreignent
les possibilités des programmes pour limiter les conséquences d'un défaut de
ces derniers. Ils renforcent aussi dans une certaine mesure le respect de
certaines règles du jeu. Ne cherchez pas à vous en défaire.

Structure du dépôt et scripts
=============================

Les outils de gestion des matchs font appel à deux scripts qui **doivent** être
présents à la racine du dépôt de chaque équipe et **éxécutables**:

- ``build.sh`` : script de compilation du programme lancé une fois au début de chaque série de matchs
- ``start.sh``: script de lancement de la stratégie, appelé sur la ligne de commande du serveur de jeu

Ces deux scripts sont écrits en bash_.

Le script de compilation peut être un script vide :

.. code-block:: bash

  #!/bin/bash

ou contenir des instructions de compilation :

.. code-block:: bash

  #!/bin/bash

  make clean
  ./configure
  make

Le script de démarrage lui, ne peut être vide. Il doit également rediriger le flux
d'entrée standard (``/dev/stdin``) vers la commande d'appel de votre programme :

.. code-block:: bash

  #!/bin/bash

  python3 ma_strategie.py < /dev/stdin

N'hésitez pas à nous demander de l'aide pour l'écriture de ces scripts essentiels au bon
déroulement de vos 24h.

.. _bash: https://en.wikipedia.org/wiki/Bash_(Unix_shell)

.. [#f1] Pour s'assurer de l'utilisation de la bonne version de l'interpréteur
         python, indiquez explicitement celle-ci dans le shebang.

.. [#f2] L'outil est particulièrement avantageux dès que l'effort de
         développement est porté par plus de 0 personne.

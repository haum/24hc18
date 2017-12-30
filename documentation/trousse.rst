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
 - Python 2
 - Python 3

Vous êtes libres d'utiliser le langage qui vous convient parmi cette liste.

Flush
=====

Le dialogue entre le programme de stratégie et le simulateur se fait à travers
des flux de données qui restent ouverts. Or les flux de données sont souvent
bufferisés pour augmenter l'efficacité des transferts en minimisant les
changements de contexte. Dans notre cas, ce mécanisme est également utile mais
il faut veiller à bien envoyer les données aux moments opportuns.

Pour cela, nous vous encourageons à trouver la commande qui permet de réaliser
un flush (c'est le terme souvent utilisé dans les API) du buffer, c'est à dire
qui envoie effectivement le contenu des mémoires tampon dans le flux.

Vous pourrez ainsi le faire juste après avoir envoyé la commande `END` et ainsi
être certains que le simulateur n'attendra pas indéfiniment des commandes qui
seraient coincées dans un buffer intermédiaire.

Bien ranger et conserver votre travail
======================================

Pour chaque match, c'est nous qui compilons/exécutons votre code sur nos
systèmes. Vous **devez** utiliser un système de gestion de version : Git.

Vous avez accès à la présente documentation qui contient la description du 
protocole de communication que vous devez utiliser.

Vous avez également accès au code source de notre simulateur, libre à vous de le
compiler sur vos pc pour tester vos stratégies.

Quel que soit le langage que vous choisirez, il est préférable que votre code
soit facilement identifiable soit par son nom, soit par une méthode de rangement 
efficace.

quelques exemples :

 - [noméquipe]_[scénario].extension
 - /noméquipe/scénario/votrecode

Dernier avertissement
=====================

.. warning:: Lors des matchs, votre code tourne en environnement clos (jail).
   Essayer de s'échapper c'est être immédiatement disqualifié. L'environnement
   complet pourra être redémarré à l'envi par le système de jeu pour s'assurer
   de bon respect des consignes (à sa seule discrétion).

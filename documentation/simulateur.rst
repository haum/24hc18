==========
Simulateur
==========

Pour vous aider dans la vérification de vos stratégies, outre vos propres et
indispensables essais manuels ou automatiques, nous vous fournissons les sources
du simulateur qui constitue l'arène de jeu. Ce chapitre fournit les instructions
qui permettent de jouer un match avec ce simulateur.

Le programme utilisant des primitives bas-niveau, il n'est pas multiplateforme.
Pour ceux qui le souhaitent, nous pouvons fournir une machine virtuelle
disposant d'un système GNU/Linux et de tous les outils pour compiler et exécuter
le jeu.

Machine virtuelle
=================

.. TIP::
    La machine virtuelle que nous utiliserons pour les matchs officiels est
    strictement identique à celle qui vous sera fournie. Faites-en bon usage.

Identifiants
------------

Les identifiants pour le compte utilisateur sur la machine virtuelle sont :

 - utilisateur  : marabunta
 - mot de passe : marabunta

Envoi de votre code
-------------------

Pour commencer, il vous faut un depot `GIT` public. Il existe plusieurs services
en ligne proposant cet outil comme `<https://framagit.org>`_ ou 
`<https://github.com>`_.

Après la création de voter dépot `GIT`, vous pouvez y déposer votre code. Il
vous reste ensuite à 'cloner'  ce dépot sur la machine virtuelle. 

Si vous ne connaissez pas cet outil, nous vous recommendons de consulter la
documentation officielle : `<https://git-scm.com>`_

Utiliser le simulateur
======================

Utilisation de base
-------------------

Le programme de simulation est l'exécutable `marabunta_engine`. Exemple
d'utilisation :

``./marabunta_engine -s scenarii/food_near_nest_1t_5s.cfg ./my_strategy.py``

Avec cette commande, le simulateur est lancé avec le scénario inscrit dans le
fichier `scenarii/food_near_nest_1t_5s.cfg` (argument `-s`) et le programme de
stratégie `./my_strategy.py`.

Si le scénario le permet, il est possible de lancer plusieurs programmes de
stratégie sur le même terrain en listant le nom de chaque programme à la fin de
la commande. Si vous avez vous-même des arguments, notamment pour paramétrer vos
tests, n'oubliez pas d'entourer la commande de guillemets.

Déverminage
-----------

Il est possible d'avoir une trace du dialogue entre le simulateur et votre
programme de stratégie pour analyser le comportement. Pour cela, il existe
l'option `-d` qui prend en argument le numéro (commençant à 0) de la stratégie à
espionner.

Extrait de trace en milieu de partie :

  | 1 @ BEGIN NEST
  | 1 @ STOCK 0
  | 1 @ MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
  | 1 @ END
  | 1 . ANT_NEW 0
  | 1 . SET_MEMORY 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
  | 1 . END
  | 0 @ BEGIN ANT
  | 0 @ TYPE 0
  | 0 @ MEMORY 0 0
  | 0 @ STAMINA 198
  | 0 @ STOCK 0
  | 0 @ SEE_FOOD 1 NEAR 5
  | 0 @ SEE_ANT 2 NEAR FRIEND 180
  | 0 @ SEE_ANT 3 NEAR ENEMY 160
  | 0 @ SEE_ANT 4 NEAR FRIEND 200
  | 0 @ END
  | 0 . EXPLORE
  | 0 . END
  | 0 ! Random kill
  | 0 ! Process died
  | 0 @ BEGIN ANT
  | 0 @ TYPE 0
  | 0 @ MEMORY 0 14
  | 0 @ STAMINA 312
  | 0 @ STOCK 22
  | 0 @ END
  | 0 . EXPLORE
  | 0 . END


Le premier chiffre indique sur quelle instance de votre programme de stratégie
est exécutée l'entité. Si vous sauvez par erreur des informations dans le
programme de stratégie, ce chiffre précisant l'alternance des instances peut
vous aider à détecter l'erreur.

Le second caractère peut être un des symboles suivants et indique la
signification du reste de la ligne :

=== ==
`@` Ce que le simulateur dit au programme de stratégie.
`.` Ce que le simulateur a reçu du programme de stratégie.
`!` Des informations complémentaires du simulateur.
=== ==

Scénario
--------

Il est possible de créer votre propre scénario pour réaliser vos tests : il
s'agit de décrire l'état initial de la simulation dans un fichier texte qui sera
interprété par le simulateur. Le contenu d'un scénario est décrit dans un
chapitre à part.

Utilisation avancée
-------------------

Le simulateur dispose d'options avancées comme la récupération de données à
travers le réseau. Ces options nous permettent par exemple d'afficher les
entités sur une carte. Cependant ces éléments ne sont pas documentés. Libre à
vos d'étudier notre code pour en tirer profit si besoin.

De même, puisque vous disposez des sources du simulateur, vous avez la
possibilité de modifier notre code localement pour instrumenter vos programmes,
créer des situations qui ne seraient pas disponibles à travers un scénario, ou
toute autre fin spécifique. Gardez cependant une copie de l'original pour
effectuer d'autres tests dans les mêmes conditions que les tournois.

Bugs
----

Même si nous avons réalisé de nombreux matchs de test, notre simulateur n'est
peut-être pas exempt de bugs. N'hésitez pas à nous les signaler.  Dans le cas où
ce bug deviendrait bloquant, vous disposez des sources pour nous aider à le
résoudre.

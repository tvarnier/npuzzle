# NPUZZLE [[Sujet](https://github.com/tvarnier/npuzzle/blob/master/npuzzle.en.pdf)]

<br>

L'objectif de ce projet est de créer un programme qui résout le [jeu de taquin](https://fr.wikipedia.org/wiki/Taquin) en utilisant un algorithme de recherche [A*](https://fr.wikipedia.org/wiki/Algorithme_A*)

<br>

```
 ❯ ./npuzzle -gen 4
=====  Start  =====
04 10 01 11
14 05 08 06
02 12 15 07
__ 13 09 03
=====  Target =====
01 02 03 04
12 13 14 05
11 __ 15 06
10 09 08 07
----- WIN -----
Complexity in Time :: 5157
Complexity in Size :: 10098
Number of Moves    :: 62
01 02 03 04
12 13 14 05
11 __ 15 06
10 09 08 07
 >> Steps and more in file "results.txt"
```

<br>

## LANCER LE PROJET

<br>

Lancer la commande `make` pour compiler le projet, Puis :

**`./npuzzle`** avec les arguments :
 * `-alg <SEARCH_ALGORITHM>` pour choisir un algorithme de recherche (astar par défaut) parmis : 
   * astar ( [A*](https://fr.wikipedia.org/wiki/Algorithme_A*) )
   * uniform ( [Uniform-Cost](https://www.educative.io/answers/what-is-uniform-cost-search) )
   * greedy ( [Greedy](https://en.wikipedia.org/wiki/Greedy_algorithm) )
 * `-hr <HEURISTIC>` pour choisir l'heuristique (manhattan par défaut) parmis : 
   * manhattan ( [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) )
   * linearConflict ( [Linear Conflict](https://medium.com/swlh/looking-into-k-puzzle-heuristics-6189318eaca2) )
   * misplaced ( [Hamming Distance](https://en.wikipedia.org/wiki/Hamming_distance) )
 * `-gen <GENERATION_LENGTH> [<GENERATION_ITERATION>]` pour generer un puzzle de départ avec :
   * une taille `<GENERATION_LENGTH>` comprise entre 3 et 256 comrpis
   * un nombre de deplacement de `<GENERATION_ITERATION>`, 10000 par défault
 * `-tgt <TARGET_TYPE or TARGET_FILE>` pour definir la disposition des tuiles de l'objectif (snail par défault) :
     * snail | forme escargot
        ```
        01 02 03 04
        12 13 14 05
        11 __ 15 06
        10 09 08 07
        ```
     * ordered | dans l'ordre
        ```
        01 02 03 04
        05 06 07 08
        09 10 11 12
        13 14 15 __
        ```
     * random | aléatoire, exemple :
        ```
        03 09 14 12
        02 13 08 07
        01 15 10 05
        __ 11 06 04
        ```
     * Un fichier sous la forme :
        ```
        4
         9  7  3  0
         4  1  2 10
        11  6  8 13
         5 12 15 14
        ```
 * `-visu` pour lancer le visualisateur :
   * `◀` etape precedente
   * `▶` etape suivante
   * `q` pour quitter

<br>

*Le programme lira sur l'entrée standard si `-gen` n'est pas utilisé pour definir d'état de départ*

*le script `res_npuzzle-gen.py` permet de generer des puzzle que ce soit pour l'état de départ ou l'état objectif, exemple : `./res_npuzzle-gen.py 3 -s`*

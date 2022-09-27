# NPUZZLE [[Sujet](https://github.com/tvarnier/npuzzle/blob/master/npuzzle.en.pdf)]

<br>

L'objectif de ce projet est de créer un programme qui résout le [jeu de taquin](https://fr.wikipedia.org/wiki/Taquin) en utilisant un algorithme de recherche [A*](https://fr.wikipedia.org/wiki/Algorithme_A*)

<br>

<div>
  <img src="https://github.com/tvarnier/rubik/blob/master/img/visualizer.gif" align="middle">
</div>

<br>

## LANCER LE PROJET

<br>

Lancer la commande `make` pour compiler le projet.

Il suffit ensuite de lancer la commande `./rubik [-v] [-s <nbrIterations> ou <rotations>]`
 * `-v` pour lancer le visualisateur
 * De base le programe fait un melange du Rubik de 30 iterations sinon :
   * `-s <nbrIterations>` pour choisir le nombre d'iterations
   * `<rotations>` indiquer une serie de rotations precise, exemple `"R U2 L' B' D2 F"`

<br>

les rotations disponibles sont `F` `R` `U` `B` `L` `D` pour Front / Right
/ Up / Back / Left / Down (Voir [notations](https://www.francocube.com/notation) pour une representation visuelle)

Auquels on peut ajouter `'` pour signifier l'inverse (ex: `F'`) et `2` pour un demi-tour (ex: `F2`)

<br>

L'algorithme utilisé est grandement inspiré par celui de [Kociemba](http://kociemba.org/cube.htm)

Il permet de resoudre rapidement un cube en 30 rotations ou moins maximum

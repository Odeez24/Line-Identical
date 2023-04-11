Projet d’Algorithmique
=====================


# Sommaire
* Description de l' algorithme et utilisation
* Implémentation de l'algorithme
* Problème durant la conception
* Conclusion

## Description de l' algorithme et utilisation
### Description
L’algorithme a deux utilisations différentes :

* La première est lorsqu' un seul fichier est passée en argument du programme, affiche pour chaque ligne de texte non vide du fichier, apparaissant au moins deux fois dans le fichier, les numéros des lignes où elle apparaît et le contenu de la ligne.
* La seconde est lorsque au minimum deux fichier sont passées en argument du programme, affiche pour chaque ligne de texte non vide apparaissant au moins une fois dans tous les fichiers, le nombre d’occurrences de la ligne dans chacun des fichiers et le contenu de la ligne.

### Utilisation
* Appel à exécutable "./lnid" puis les fichiers et/ou les options peuvent être mise à n'importe quel endroit.
	* Liste des options

|
|-|--|:--|
|-h|--help| Affiche sur la sortie standard la description du programme et l’utilisation de celui-ci|
|-u|--uppercase|Transforme tous les caractères traitée et étant des caractères minuscules en majuscules|
|-f [option]|--filter=[option]|Applique l'un des douze test d’appartenance à une catégorie de caractères is... de l’en-tête standard <ctype.h> passée en argument à filter (Remplacer [option] par le nom du filtre comme "isalnum")|
* Si l'option help est appelée elle s’exécutera puis le programme s’arrêtera proprement.
* L'option uppercase ne s'applique seulement sur les caractères alphabétiques minuscules. 
 * Dans le cas où plusieurs filtre sont appelés seul le dernier filtre appelé sera appliquer. 

#### Tous les arguments passés à l’exécutable et qui ne sont pas des options prises en charge doivent être un fichier valide sinon une erreur a l’exécution se produira.

## Implémentation 
### Problématiques 
Le programme avait plusieurs obligations et problématique pour son fonctionnement

1. Le programme devait pouvoir lire sur un fichier une chaîne de caractère dont on ne connaissait pas ça longueur.
2. Il devait aussi pouvoir enregistrer des occurrences de chaque chaîne de caractère selon le nombre de fichier que l'on ne connaît qu'à l’exécution ou enregistrer des numéros de lignes qui lier a un fichier qui peut avoir un nombre de ligne très petit ou très long.
3. Il devait donc pouvoir enregistrer toute ses données en prenant le moins de temps lors de recherche.
4. Et faire cela en étant le plus rapide et en prenant le moins d'espace mémoire

### Solutions
  Mon implémentations ce découpe en 5 modules.

1. Les deux premiers modules sont le module hashtable et holdall qui sont ceux fournis et étudier en cours et qui n'ont pas été modifier.
2. Le troisième module est le module opt qui a pour but de gérer les options, il possède trois fonctions :
	1. opt_gen qui alloue les ressources pour gérer une option.
	2. opt_dispose qui libère les ressources alloués pour une option.
	3. opt_init qui elle initialise les options et sépare les arguments entre les options et les fichiers.

3. Le quatrième est un module polymorphe de tableaux dynamique da 
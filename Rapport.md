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
  Mon implémentations ce découpe en 5 modules, plus le main.

#### Les modules

1. Les deux premiers modules sont le module hashtable et holdall qui sont ceux fournis et étudier en cours et qui n'ont pas été modifier.
2. Le troisième module est le module opt qui a pour but de gérer les options, il possède trois fonctions :
	1. opt_gen qui alloue les ressources pour gérer une option.
	2. opt_dispose qui libère les ressources alloués pour une option.
	3. opt_init qui elle initialise les options et sépare les arguments entre les options et les fichiers.

3. Le quatrième est un module polymorphe de tableaux dynamique da qui contiens des référence vers des objets.
	1. Il possède les fonctions classiques telle que da_empty, da_add, da_dispose, da_ref qui renvoie la référence d'indice i du da et da_lenght.
	2. La dernière fonction est da_dispose_element qui libère les zones mémoires pointées par les référence contenu dans da.
4. Le dernier module est une ds qui est un module de chaîne de caractère dynamique qui ne stocke aucune donnée (aucune allocation n'est a effectuer sur l'élément à ajouter).
	1. Il posséde les même fonction classique que le module da, donc ds_empty, ds_dispose, ds_add, ds_ref, ds_length.

#### Le main
Dans le main se trouve une nouvelle structure nommé cntxt qui sera le context d'utilisation pour nos fonctions des différents modules ou des fonctions du main.
	Elle contient 3 champs les deux premiers sont liés aux options qui sont déclarées ensuite.

1. Le champ filter prend la fonction de filtre passer en argument (ex : l'option --filter=isalnum alors le champ filter du contexte prendra alors la fonction isalnum de la bibliothèque <ctype.h>).
2. Le champ transform lui prend la fonction liés a l'option --uppercase, il prend donc la fonction toupper si l'option est passée en argument a l’exécution.

Le dernier champs lui est un da qui suite à l'appel de opt_init contiendra les arguments passé au programme qui ne sont pas des options. Donc si une option est mal écrite alors une erreur lors de l’exécution se produira.

Ensuite sont déclarées les options par des appels à opt_gen, qui prend comme paramètre les deux nom de l'option, sa description ainsi que sa fonction qui permet d'initialiser le champ du contexte lier.

Puis toute nos structure qui seront utiliser sont alloues comme hashtable, holdall, un da et un ds ainsi que un autre da pour la structure cntxt.

Le corps du main est une boucle qui appelle la fonction addline qui enregistre une ligne du fichier ouvert en lecture est l'enregistre dans un ds.
Puis une recherche dans la table de hachage se fait et selon le résultat soit la ligne est ajouter à la table sont le tableau de compteur lié a la ligne est incrémenté ou affecté.

Ensuite pour chaque chaîne contenu dans la table si il respecte la condition lorsque il y a un ou plusieurs fichier alors elle est affiché précéder des éléments du tableau de compteur qui lui sont liés.

Pour finir une libération de toute la mémoire alloués est effectuer.

## Problème durant la conception 

# Rapport projet Algorithmique 3 2022/2023

# Sommaire
* Description de l'algorithme et utilisation
* Implémentation de l'algorithme
* Exemple d’exécution
* Problème durant la conception
* Conclusion

## Description de l'algorithme et utilisation
### Description
L’algorithme a deux utilisations différentes :

* La première: lorsqu'un seul fichier est passée en argument du programme, affiche pour chaque ligne de texte non vide du fichier, apparaissant au moins deux fois dans le fichier, les numéros des lignes où elle apparaît et le contenu de la ligne.
* La seconde: lorsque au minimum deux fichiers sont passés en argument du programme, affiche pour chaque ligne de texte non vide apparaissant au moins une fois dans tous les fichiers, le nombre d’occurrences de la ligne dans chacun des fichiers et le contenu de la ligne.

### Utilisation
* Appel à exécutable "./lnid" les fichiers et les différentes options peuvent être placé à n'importe quel endroit dans les arguments.
	* Liste des options

|
|-|--|:--|
|-h|--help| Affiche sur la sortie standard la description du programme et l’utilisation de celui-ci|
|-u|--uppercase|Transforme tous les caractères traités et étant des caractères minuscules en majuscules|
|-f [option]|--filter=[option]|Applique l'un des douze tests d’appartenance à une catégorie de caractères is... de l’en-tête standard <ctype.h> passée en argument à filter (Remplacer [option] par le nom du filtre comme "isalnum")|
* Si l'option help est appelée, elle s’exécutera puis le programme s’arrêtera proprement.
* L'option uppercase ne s'applique seulement sur les caractères alphabétiques minuscules. 
 * Dans le cas où plusieurs filtres sont appelés, seul le dernier filtre appelé sera appliquer. 

#### Tous les arguments passés à l’exécutable et qui ne sont pas des options prises en charge doivent être un fichier valide sinon une erreur à l’exécution se produira.

## Implémentation 
### Problématiques 
Le programme avait plusieurs obligations et problématique pour son fonctionnement.

1. Le programme devait pouvoir lire sur un fichier une chaîne de caractère dont on ne connaissait pas sa longueur.
2. Il devait aussi pouvoir enregistrer des occurrences de chaque chaîne de caractère selon le nombre de fichiers que l'on ne connaît qu'à l’exécution ou enregistrer des numéros de lignes qui lier à un fichier qui peut avoir un nombre de lignes très petit ou très long.
3. Il devait donc pouvoir enregistrer toutes ses données en prenant le moins de temps lors de recherche.
4. Et faire cela en étant le plus rapide et en prenant le moins d'espace mémoire

### Solutions
  Mon implémentation se découpe en 5 modules, plus la main.

#### Les modules

1. Les deux premiers modules sont le module hashtable et holdall qui sont ceux fournis et étudier en cours et qui n'ont pas été modifier.
2. Le troisième module est le module opt qui a pour but de gérer les options, il possède trois fonctions :
	1. opt_gen qui alloue les ressources pour gérer une option.
	2. opt_dispose qui libère les ressources allouées pour une option.
	3. opt_init qui elle initialise les options et sépare les arguments entre les options et les fichiers.

3. Le quatrième est un module polymorphe de tableaux dynamique da qui contient des références vers des objets.
	1. Il possède les fonctions classiques telles que da_empty, da_add, da_dispose, da_ref qui renvoie la référence d'indice i du da et da_lenght.
	2. La dernière fonction est da_dispose_element qui libère les zones mémoires pointées par les références contenues dans da.
4. Le dernier module est une ds qui est un module de chaîne de caractère dynamique qui ne stocke aucune donnée (aucune allocation n'est à effectuer sur l'élément à ajouter).
	1. Il possède les mêmes fonctions classiques que le module da, donc ds_empty, ds_dispose, ds_add, ds_ref, ds_length.
5. Lorsque l'ont créé un da ou un ds une zone mémoire d'une certaine taille est allouée la taille de cette zone mémoire pour da ou ds sont une macroconstante nommé DA__CAPACITY_MIN et DS__CAPACITY_MIN où DA_CAPACITY_MIN vaut 4, car après plusieurs tests dans un même texte c'est la récurrence moyenne d'une ligne, il y aura de la réallocation obligatoire sur des textes qui assez gros comme les misérables où dans ce texte des lignes apparaissent plus de 20 fois, mais j'ai choisi 4 comme valeur initiale puisque cela permet de sauver de la mémoire tout en restant efficace sur des petits textes.
DS__CAPACITY_MIN lui est à 32, car 32 caractères  par ligne permet d'être assez efficace sur des textes avec de longue ligne sans perdre en efficacité sur des textes avec des petites lignes.

#### Le main

* Dans le main se trouve une nouvelle structure nommée cntxt qui sera le contexte d'utilisation pour nos fonctions des différents modules ou des fonctions du main.
	Elle contient 3 champs, les deux premiers sont liés aux options qui sont déclarées ensuite.

1. Le champ filter prend la fonction de filtre passé en argument (ex : l'option --filter=isalnum alors le champ filter du contexte prendra alors la fonction isalnum de la bibliothèque <ctype.h>).
2. Le champ transform lui prend la fonction liée à l'option --uppercase, il prend donc la fonction toupper si l'option est passée en argument à l’exécution.

* Le dernier champ lui est un da qui suite à l'appel de opt_init contiendra les arguments passés au programme qui ne sont pas des options. Ainsi, si une option est mal écrite, alors une erreur lors de l’exécution se produira.

* Ensuite sont déclarées les options par des appels à opt_gen, qui prend comme paramètre les deux noms de l'option, sa description ainsi que sa fonction qui permet d'initialiser le champ du contexte lié.

 * Puis toutes nos structures qui seront utiliser sont allouées comme hashtable, holdall, un da et un ds ainsi qu'un autre da pour la structure cntxt.

* Le corps du main est une boucle qui appelle la fonction addline qui enregistre une ligne du fichier ouvert en lecture est l'enregistre dans un ds.
Puis une recherche dans la table de hachage se fait et selon le résultat soit la recherche est positive donc le compteur lié à la ligne est incrémentée, soit si nous somme sur le premier fichier la ligne est ajoutée à la table et sont le tableau de compteur est ajouté lui aussi.

* Ensuite, pour chaque chaîne contenue dans la table, si elle respecte la condition selon s'il y a un ou plusieurs fichiers,  alors elle est affichée précéder des éléments du tableau de compteur qui lui sont liés.

* Pour finir, une libération de toute la mémoire allouée est effectuée.

## Exemple d’exécution
#### Exécution sur le texte bobdylan_iwysb :
* Résultat

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-25-52.png) 

* Avec Valgrind 

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-26-39.png)

* Et time

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-27-18.png)

#### Les misérables
* Ceci n'est qu'un échantillon du résultat donné dû à la longueur du résultat

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-28-19.png)

* Valgrind 

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-30-05.png)

* Time

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-01%2020-30-56.png)

#### Fichier de 120 mo
* Valgrind 

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-07%2016-31-41.png)

* Time

![Capture d’écran du 2023-05-07 16-34-15.png](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-07%2016-34-15.png)

#### Comparaison avec le programme du groupe Haddag Edouard, Théo Renaux Verdiere

* Valgrind

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-07%2016-34-35.png)

* Time

![](/home/florent/Images/Captures%20d%E2%80%99%C3%A9cran/Capture%20d%E2%80%99%C3%A9cran%20du%202023-05-07%2016-33-45.png)

## Problème durant la conception 
1. Le module ds est dû au fait que je n'ai pas réussi à utiliser efficacement le module da pour la lecture de ligne, cela me posait des problèmes de mémoires qui sont pareillement réglées.
2. Le module opt est lui le module qui fut le plus compliqué à faire du au fait que celui-ci est totalement utilisable en dehors de ce projet et que beaucoup de cas sont à traiter.
3. Mon plus gros problème est une sur-utilisation de la mémoire que je n'arrive pas ni à expliquer ni à corriger. Ce problème de mémoire est un facteur de 28,30 sur la taille du texte.

#### Attention, avec des fichiers trop gros ou en trop grand nombre, dû à la surutilisation de mémoire, cela peut résulter par un crash de l'ordinateur.

## Conclusion
* Mon programme est donc formé de 3 modules (hors main) dont 2 sont totalement polymorphes et réutilisables dans de futur projet.
* Le temps d’exécution de mon programme est suffisant en comparaison avec celui de Haddag Edouard et Théo Renaux Verdiere qui lui a beaucoup plus d'option, mais comme je m'en dotais mon programme à un réel problème en termes de gestion de mémoire.
* Mon programme a pour le moment 2 grands axe d'améliorations :
	* Le problème d'utilisation de la mémoire qui vient très probablement de ma manière de lire et de stocké temporairement les caractères d'une ligne.
	* Ma gestion d'option : possibilité d'ajouter une fonctionnalité pour essayer de reconnaître si l'argument passer en paramètre est une option simplement mal tapée pour lors de l'erreur aidée l'utilisateur.
* Je tiens à remercier Haddag Edouard, Théo Renaux Verdiere, Louis Dumontier Louis et Salles Théo pour leur aide à la résolution de problème sur mon programme ainsi que de m'avoir fourni leurs exécutables pour faire des comparaisons.



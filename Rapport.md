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

  
//  ds.h : partie interface d'un module  pour la spécification d'un tableau
//    dynamique de caractères.

#ifndef DS__H
#define DS__H

#include <stdlib.h>
#include <stdio.h>
#include "stdint.h"

//  Fonctionnement général :
//  - la structure de données stocke des caractères.
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « ds * » ou « ds ** » ont
//      un comportement indéterminé lorsque ce paramètre ou sa déréférence
//      n'est pas l'adresse d'un contrôleur préalablement renvoyée avec succès
//      par la fonction ds_empty et non révoquée depuis par la
//      fonction ds_dispose ;
//  - aucune fonction ne peut ajouter NULL à la structure de données;
//  - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire avec aucun problème de la sorte.

//  struct ds, ds : Type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un tableau dynamique de caractères.
typedef struct ds ds;

//  ds_empty : Tente d'allouer les ressources nécessaires pour gérer un nouveau
//    tableau dynamique de caractères initialement vide.
//    Renvoie NULL en cas de dépassement de
//    capacité. Renvoie sinon un pointeur vers le contrôleur associé au tableau.
extern ds *ds_empty();

//  ds_dispose : Libère les ressources allouées à la gestion du tableaux
//    dynamique associé à *aptr puis affecte NULL à *aptr.
//    Sans effet si *aptr vaut NULL,
extern void ds_dispose(ds **aptr);

//  ds_add : Ajoute le caractères c selon la méthode de l'ajout en bout de
//    chemin
//   Renvoie -1 en cas de dépassement de capacité ; renvoie sinon 0.
extern int ds_add(ds *p, const char s);

//  ds_ref : Renvoie le caractère d'incide i du tableau pointé par p.
extern char ds_ref(ds *p, size_t i);

//  ds_length : Renvoie la longueur du tableaux associés à p.
extern size_t ds_length(ds *d);
#endif

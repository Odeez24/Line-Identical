//  da.h : partie interface d'un module polymorphe pour la spécification d'un
//    tableau dynamique.

#ifndef DA__H
#define DA__H

#include <stdlib.h>
#include <stdio.h>

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « da * » ou « da ** » ont
//      un comportement indéterminé lorsque ce paramètre ou sa déréférence
//      n'est pas l'adresse d'un contrôleur préalablement renvoyée avec succès
//      par la fonction pqueue_empty et non révoquée depuis par la
//      fonction ad_dispose ;
//  - aucune fonction ne peut ajouter NULL à la structure de données ;
//  - les fonctions de type de retour « void * » renvoient NULL en cas d'échec.
//      En cas de succès, elles renvoient une référence actuellement ou
//      auparavant stockée par la structure de données ;
//  - l'implantation des fonctions dont la spécification ne précise pas qu'elles
//      doivent gérer les cas de dépassement de capacité ne doivent avoir
//      affaire avec aucun problème de la sorte.

//  struct da, da : Type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer un tableau dynamique d'objets
//    quelconques.
typedef struct da da;

//  da_empty : Tente d'allouer les ressources nécessaires pour gérer un nouveau
//    tableau dynamique initialement vide.
//    Renvoie NULL en cas de dépassement de
//    capacité. Renvoie sinon un pointeur vers le contrôleur associé au tableau.
extern da *da_empty();

//  da_dispose : Libère les ressources allouées à la gestion du tableaux
//    dynamique associé à *aptr puis affecte NULL à *aptr.
//    Sans effet si *aptr vaut NULL,
extern void da_dispose(da **aptr);

//  da_add : Renvoie NULL si ref vaut NULL. Tente sinon d'ajouter la référence
//    selon la méthode de l'ajout en bout de chemin renvoie NULL en cas de
//    dépassement de capacité ; renvoie sinon ref.
extern void *da_add(da *p, void *ref);

//  da_ref : Renvoie la référence d'incide i du tableau pointé par p.
//    Renvoie NULL si p est vide ou si i est supérieur a la longueur de p
//    sinon.
extern void *da_ref(da *p, size_t i);

//  da_mod_ref : Modifie la valeur de la case d'indice i du tableau dynamique
//    pointer par p par ref.
//    Renvoie NULL si ref vaut, si i est supérieur a la longueur de p ou si vaut
//    null sinon renvoie ref.
extern void *da_mod_ref(da *p, size_t i, void *ref);

//  da_length : Renvoie la longueur du tableaux associés à p.
extern size_t da_length(da *d);

//  da_capacity : Renvoie la capaciter du tableaux associés à p.
extern size_t da_capacity(da *d);

#endif

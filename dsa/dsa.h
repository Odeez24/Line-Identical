//  dsa.h : partie interface d'un module polymorphe pour la spécification d'un
//    module utilisant des tableaux dynamique pour enregistrer une chaîne de
//    caractére et des entiers naturels positif que l'on nommera DSA.

#ifndef DSA__H
#define DSA__H

#include <stdlib.h>
#include <stdio.h>
#include "../da/da.h"

//  Fonctionnement général :
//  - la structure de données ne stocke pas d'objets mais des références vers
//      ces objets. Les références sont du type générique « void * » ;
//  - si des opérations d'allocation dynamique sont effectuées, elles le sont
//      pour la gestion propre de la structure de données, et en aucun cas pour
//      réaliser des copies ou des destructions d'objets ;
//  - les fonctions qui possèdent un paramètre de type « dsa * » ou « dsa ** »
//      ont un comportement indéterminé lorsque ce paramètre ou sa déréférence
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

//  struct dsa, dsa : Type et nom de type d'un contrôleur regroupant les
//    informations nécessaires pour gérer une structure utilisant des tableaux
//    dynamique d'objets quelconques.
typedef struct dsa dsa;

//  dsa_empty : Tente d'allouer les ressources nécessaires pour gérer un nouveau
//    DSA initialement vide.
//    Renvoie NULL en cas de dépassement de capacité. Renvoie sinon un pointeur
//    vers le contrôleur associé au DSA.
extern dsa *dsa_empty();

//  dsa_dispose : Libère les ressources allouées à la gestion du DSA associé à
//    *aptr puis affecte NULL à *aptr.
//    Sinon sans effet si *aptr vaut NULL.
extern void dsa_dispose(dsa **aptr);

//  dsa_add : On suppose que le fichier filename est ouvert en lecture. Tente
//    sinon de lire une ligne du filename et de l'ajouter a un tableaux
//    dynamique de DSA et ajoute l'entier num au tableaux CPT.
//    Renvoie une valeur négative en cas de dépassement de capacité ou de
//    probleme de lecture sur le fichier, renvoie une valeur positive si on est
//    a la fin du ficher, renvoie 0 sinon;
extern int dsa_add(dsa *p, FILE *filename, size_t *num);

//  dsa_add_cpt : Ajoute cpt au tableau CPT du DSA pointer par p.
//    Renvoie NULL en cas de dépassement de capacité, sinon renvoie cpt.
extern void *dsa_add_cpt(dsa *p, size_t *cpt);

//  dsa_ref : Renvoie la référence d'incide i du tableau pointé par p.
//  Renvoie NULL si p est vide ou si i est supérieur a la longueur de p.
extern char *dsa_ref_string(dsa *p, size_t i);

//  dsa_ref : Renvoie la référence d'incide i du tableau pointé par p.
//  Renvoie NULL si p est vide ou si i est supérieur a la longueur de p.
extern void *dsa_ref_cpt(dsa *p, size_t i);

//  dsa_length : Renvoie la longueur du tableaux de caractére associée à p.
extern size_t dsa_length_string(dsa *p);

//  dsa_length : Renvoie la longueur du tableaux d'entier associée à p.
extern size_t dsa_length_int(dsa *p);

//  dsa_cpt : Renvoie le pointer vers le da cpt du DSA pointer par p.
da *dsa_cpt(dsa *p);

#endif

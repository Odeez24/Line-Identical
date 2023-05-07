// Partie interface d'un module pour la gestion d'otion (opt)

#ifndef OPT__H
#define OPT__H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//  struct opt opt : type et nom d'un contrôleur regroupant les informations
//    nécessaires pour gérer une option
typedef struct opt opt;

//  returnopt : énumération des différentes valeurs de retour des fonctions du
//    module opt
typedef enum {
  NO_OPT,
  ERR_OPT,
  ERR_ADD,
  ERROR,
  HELP,
  SUCCESS,
} returnopt;

//  opt_gen : Alloue les ressources nécessaires pour la gestion d'une nouvelle
//    option et en initialisant, c'est attributs avec les paramètres passés en
//    paramètres.
//    Renvoie NULL en cas de dépassement de capacité, sinon renvoie un pointer
//    vers la zone mémoire allouer.
extern opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
    bool arg, int (*fun)(const void *, const void *));

//  opt_dispose : Libère les ressources allouées à la gestion d'une option
//    associés a *aptr, puis affecte NULL à *aptr.
//  Sans effet si *aptr vaut NULL.
extern void opt_dispose(opt **aopt);

//  opt_parse : Prend en paramètres le nombre d'arguments, letableau contenant
//    les arguments, le tableau contenant les options supporter, le nombre
//    d'options, un contexte, une description de ce que fait le programme et de
//    comment l'utiliser ainsi qu'une fonction d'ajout.
//    Le contexte est là où seront mises les fonctions lier aux options et où
//    la fonction d'ajout stockera tous les arguments qui ne sont pas des
//    options supporter.
//    Si un argument est une option, alors appelle la fonction liée à l'option
//    pour l'initialiser dans le contexte, si l'argument est la commande d'aide
//    affiche sur la sortie standard la desc et usage et s'arrête, sinon appelle
//    la fonction d'ajout avec comme paramètre le contexte et l'argument.
//    L'option d'aide (--help; -h) est déjà implémentée..
//  Renvoie NO_OPT si aucun des arguments est une option, HELP si un des
//    arguments est l'option d'aide, SUCCESS en cas de succès. Sinon renvoie
//    ERR_OPT si une erreur est survenue lors de l'exécution de la fonction
//    d'initialisation d'une option, ERR_ADD en cas d'erreur lors de l'appel à
//    la fonction d'ajout ou ERROR en cas de dépassement de capacité.
extern returnopt opt_init(int argc, const char **argv, opt **optsupp,
    size_t nbopt, void *cntxt, const char *desc, const char *usage,
    void *(*fun)(void *, const void *));

#endif

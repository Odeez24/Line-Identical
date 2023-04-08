// Partie interface d'un module pour la gestion d'otion (opt)

#ifndef OPT__H
#define OPT__H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//  struct opt opt : type et nom d'un contorleur regroupant les informations
//    nécessaires pour gérer une option
typedef struct opt opt;

//  returnopt : énumération des différente valeur de retour des fonction du
//    module opt
typedef enum {
  NO_OPT,
  ERR_OPT,
  ERR_ADD,
  ERROR,
  HELP,
  SUCCESS,
} returnopt;

//  opt_gen : Alloue les ressources nécessaires pour la gestion d'une nouvel
//    option et en initialisant c'est attributs avec les paramétre passer en
//    paramétre.
//    Renvoie NULL en cas de dépassement de capacité, sinon renvoie un pointer
//    vers la zone méoire allouer.
extern opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
    bool arg, int (*fun)(const void *, const void *));


//  opt_dispose : Libére les ressources allouées à la gestion d'un option
//    associés a *aptr, puis affecte NULL à *aptr.
//  Sans effet si *aptr vaut NULL.
extern void opt_dispose(opt **aopt);

//  opt_parse : Prend en paramétre le nombre d'argument ainsi que le
//    tableau contenant les arguments , le tableaux contenant les options ainsi
//    que le nombre d'option, un context, une description de ce que fait le
//    programme et de comment l'utiliser ainsi qu'une fonction d'ajout. Si un
//    argument est une option appele la fonction de l'option pour l'initialiser,
//    si l'argument est la commande d'aide affiche sur la sortie standart la
//    desc et usage et s'arrete, sinon appelle la fonction d'ajout avec comme
//    paramétre le contexe et l'argument qui renvoie NULL en cas d'echec.
//  Renvoie NO_OPT si aucun des argument est une option, HELP si un des
//    arguments est l'option d'aide, SUCCESS en cas de succés. Sinon renvoie
//    ERR_OPT si une erreur est survenu lors de l'exécution de la fonction
//    d'initialisation d'une option, ERR_ADD en cas d'erreur lors de l'appel à
//    la fonction d'ajout ou ERROR en cas de dépassement de capacité.
extern returnopt opt_init(int argc, const char **argv, opt **optsupp,
    size_t nbopt, void *cntxt, const char *desc, const char *usage,
    void *(*fun)(void *, const void *));

#endif

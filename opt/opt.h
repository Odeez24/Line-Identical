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
  ERR_T,
  HELP,
  SUCCESS,
} returnopt;

//  opt_gen : Alloue les ressources nécessaires pour la gestion d'une nouvel
//    option et en initialisant c'est attributs avec les paramétre passer en
//    paramétre.
//    Renvoie NULL en cas de dépassement de capacité, sinon renvoie un pointer
//    vers la zone méoire allouer
extern opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
    bool arg, int (*fun) (const void *, const void *));

//  opt_parse : Prend en paramétre les nombre d'argument passer ainsi que le
//    tableau contenant les arguments  et les sépares dans deux tableaux passer
//    en paramettre l'un contenant les options passer en paramétre et qui sont
//    dans le tableaux optsupp(lste des option supporter), ils seront dans
//    optlist, le reste sera mis dans le tableaux other.
//    Renvoie ERR_OPT en cas probléme de lecture, NO_OPT si il n'y a pas
//    d'option passer en argument, SUCCESS sinon.
extern returnopt opt_init(int argc, const char **argv, opt **optsupp,
  size_t nbopt, void *cntxt, const char *desc, const char *usage,
   void * (*fun) (void *, void *));

#endif

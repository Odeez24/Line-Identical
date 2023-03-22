//  dsa.h : partie implémentation d'un module polymorphe pour la spécification
//    d'un module utilisant des tableaux dynamique pour enregistrer une chaîne
//    de caractére et des entiers naturels positif que l'on nommera DSA.

#include "../da/da.h"
#include "dsa.h"
#include "stdint.h"

//--- Définition dsa ----------------------------------------------------------

struct dsa {
  da *string;
  da *cpt;
};

//--- Raccourcis dsa ------------------------------------------------------------
#define IS_EMPTY(p)    ((p)== NULL)
#define STRING(p)      ((p)->string)
#define CPT(p)         ((p)->cpt)

//--- Fonctions dsa -------------------------------------------------------------

dsa *dsa_empty() {
  dsa *p = malloc (sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  da *str = da_empty();
  if (str == NULL) {
    free(p);
    return NULL;
  }
  da *cpt = da_empty();
  if (cpt == NULL){
    free(p);
    free(str);
    return NULL;
  }
  STRING(p) = str;
  CPT(p) = cpt;
  return p;
}

void dsa_dispose(dsa **aptr) {
  if (*aptr == NULL) {
    return;
  }
  da_dispose(&STRING(*aptr));
  da_dispose(&CPT(*aptr));
  free(*aptr);
  *aptr = NULL;
}

//  dsa_add : Renvoie NULL si filename vaut NULL. Tente sinon de lire une ligne
//    du filename et de l'ajouter a un tableaux dynamique de DSA, renvoie NULL
//    en cas de dépassement de capacité; renvoie sinon le pointeur vers le
//    tableaux.
extern void *dsa_add_string(dsa *p, FILE *filename) {
  if (filename == NULL){
    return NULL;
  }
}


void *dsa_add_cpt(dsa *p, size_t *numlign){
  void *r = da_add(CPT(p), numlign);
  if (r == NULL){
    return NULL;
  }
  return r;
}

void *dsa_ref_string(dsa *p, size_t i) {
  return da_ref(STRING(p), i);
}

void *dsa_ref_cpt(dsa *p, size_t i) {
  return da_ref(CPT(p), i);
}

size_t dsa_length_string(dsa *p){
  return da_length(STRING(p));
}

size_t dsa_length_int(dsa *p) {
  return da_length(CPT(p));
}

//  dsa.h : partie implémentation d'un module polymorphe pour la spécification
//    d'un module utilisant des tableaux dynamique pour enregistrer une chaîne
//    de caractére et des entiers naturels positif que l'on nommera DSA.

#include "../da/da.h"
#include "dsa.h"
#include "stdint.h"

//--- Définition dsa -----------------------------------------------------------

struct dsa {
  da *string;
  da *cpt;
};

//--- Raccourcis dsa -----------------------------------------------------------
#define IS_EMPTY(p)    ((p) == NULL)
#define STRING(p)      ((p)->string)
#define CPT(p)         ((p)->cpt)

//--- Fonction interne ---------------------------------------------------------
//  line : Ajoute une ligne du fichier filename au tableaux dynamique pointer
//    par p.
//    Renvoie une valeur négative en cas de dépassement de capacité ou de
//    probleme de lecture ou une valeur positive si on arrive a la fin du
//    fichier la valeur null sinon.
int line(da *p, FILE *filename);

//--- Fonctions dsa ------------------------------------------------------------

dsa *dsa_empty() {
  dsa *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  da *str = da_empty();
  if (str == NULL) {
    free(p);
    return NULL;
  }
  da *cpt = da_empty();
  if (cpt == NULL) {
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

void *dsa_add_string(dsa *p, FILE *filename, size_t *numlign) {
  int *r;
  if ((*r =line(STRING(p), filename)) < 0) {
    return NULL;
  }
  if (da_add(CPT(p), numlign) == NULL) {
    return NULL;
  }
  return r;
}

int line(da *p, FILE *filename) {
  int c;
  while ((c = fgetc(filename)) != EOF || c != '\n') {
    if (da_add(p, &c) == NULL) {
      return -1;
    }
  }
  if (c == '\n') {
    return 0;
  }
  if (!feof(filename)) {
    return -1;
  }
  return 1;
}

void *dsa_ref_string(dsa *p, size_t i) {
  return da_ref(STRING(p), i);
}

void *dsa_ref_cpt(dsa *p, size_t i) {
  return da_ref(CPT(p), i);
}

size_t dsa_length_string(dsa *p) {
  return da_length(STRING(p));
}

size_t dsa_length_int(dsa *p) {
  return da_length(CPT(p));
}

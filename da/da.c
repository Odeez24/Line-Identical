//  da.c : partie implantation d'un module polymorphe pour la spécification
//    de tableau dynamique

#include "da.h"
#include "stdint.h"

#define TRACK fprintf(stderr, "*** %s:%d\n", __func__, __LINE__);

#define DA__CAPACITY_MIN 1
#define DA__CAPACITY_MUL 2

//--- Définition da ------------------------------------------------------------

struct da {
  const void **aref;
  size_t length;
  size_t capacity;
};

//--- Raccourcis da ------------------------------------------------------------
#define IS_EMPTY(p)    ((p)->length == 0)
#define LENGTH(p)      ((p)->length)
#define CAPACITY(p)    ((p)->capacity)

//--- Fonction interne ---------------------------------------------------------
//  line : Ajoute une ligne du fichier filename au tableaux dynamique pointer
//    par p.
//    Renvoie une valeur négative en cas de dépassement de capacité ou de
//    probleme de lecture ou une valeur positive si on arrive a la fin du
//    fichier la valeur null sinon.
int line(da *p, FILE *filename);

//--- Fonctions da -------------------------------------------------------------

da *da_empty() {
  da *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  const void **tab = malloc(DA__CAPACITY_MIN * sizeof *(p->aref));
  if (tab == NULL) {
    free(p);
    return NULL;
  }
  p->aref = tab;
  p->capacity = DA__CAPACITY_MIN;
  p->length = 0;
  return p;
}

void da_dispose(da **aptr) {
  if (*aptr == NULL) {
    return;
  }
  free((*aptr)->aref);
  free(*aptr);
  *aptr = NULL;
  return;
}

void *da_add(da *p, const void *ref) {
  if (ref == NULL) {
    TRACK
    return NULL;
  }
  if (LENGTH(p) == CAPACITY(p)) {
    if (((sizeof *(p->aref) * p->capacity)) > SIZE_MAX / DA__CAPACITY_MUL) {
      return NULL;
    }
    const void **t
      = realloc(p->aref,
        (sizeof *(p->aref) * p->capacity * DA__CAPACITY_MUL));
    if (t == NULL) {
      TRACK
      return NULL;
    }
    p->aref = t;
    p->capacity *= DA__CAPACITY_MUL;
  }
  p->aref[LENGTH(p)] = ref;
  LENGTH(p) += 1;
  return (void *) ref;
}

void *da_ref(da *p, size_t i) {
  if (p == NULL) {
    return NULL;
  }
  if (i > LENGTH(p)) {
    return NULL;
  }
  const void *v = p->aref[i];
  return (void *) v;
}

void *da_mod_ref(da *p, size_t i, void *ref) {
  if (p == NULL) {
    return NULL;
  }
  if (i > LENGTH(p)) {
    return NULL;
  }
  if (ref == NULL) {
    return NULL;
  }
  p->aref[i] = ref;
  return ref;
}

size_t da_length(da *p) {
  return IS_EMPTY(p) ? 0 : LENGTH(p);
}

size_t da_capacity(da *p) {
  return IS_EMPTY(p) ? 0 : CAPACITY(p);
}

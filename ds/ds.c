//  ds.c : partie implantation d'un  module  pour la spécification d'un tableau
//    dynamique de caractères.


#include "ds.h"

#define DS__CAPACITY_MIN 32
#define DS__CAPACITY_MUL 2

//--- Définition da ------------------------------------------------------------

struct ds {
  char *aref;
  size_t length;
  size_t capacity;
};

//--- Raccourcis da ------------------------------------------------------------
#define IS_EMPTY(p)    ((p)->length == 0)
#define LENGTH(p)      ((p)->length)
#define CAPACITY(p)    ((p)->capacity)

//--- Fonctions da -------------------------------------------------------------

ds *ds_empty() {
  ds *p = malloc(sizeof *p);
  if (p == NULL) {
    return NULL;
  }
  char *tab = malloc(DS__CAPACITY_MIN * sizeof (p->aref));
  if (tab == NULL) {
    free(p);
    return NULL;
  }
  p->aref = tab;
  p->capacity = DS__CAPACITY_MIN;
  p->length = 0;
  return p;
}


void ds_dispose(ds **sptr) {
  if (*sptr == NULL) {
    return;
  }
  free((void *)(*sptr)->aref);
  free(*sptr);
  *sptr = NULL;
  return;
}

int ds_add(ds *p, const char s) {
  if (LENGTH(p) == CAPACITY(p)) {
    if ((sizeof (p->aref) * p->capacity)> SIZE_MAX / DS__CAPACITY_MUL) {
      return -1;
    }
    char *t
      = realloc(p->aref,
        (sizeof (p->aref) * p->capacity * DS__CAPACITY_MUL));
    if (t == NULL) {
      return -1;
    }
    p->aref = t;
    p->capacity *= DS__CAPACITY_MUL;
  }
  p->aref[LENGTH(p)] = s;
  LENGTH(p) += 1;
  return 0;
}

char ds_ref(ds *p, size_t i) {
  return p->aref[i];
}

size_t ds_length(ds *p) {
  return IS_EMPTY(p) ? 0 : LENGTH(p);
}

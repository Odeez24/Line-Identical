#include <stdio.h>
#include <string.h>
#include "da.h"
#include "dsa.h"
#include "holdall.h"
#include "hashtable.h"

//  str_hashfun : l'une des fonctions de pré-hachage conseillées par Kernighan
//    et Pike pour les chaines de caractères.
static size_t str_hashfun(const char *s);

//  scptr_display_one : affiche sur la sortie standard le tableau pointer par *cpt, le caractère
//    tabulation, la chaine de caractères pointée par s et la fin de ligne.
//    Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec.
static int scptr_display_one(const char *s, da *cpt);

//  scptr_display_mult : affiche sur la sortie standard le chaque éléments
//    tableau d'entier pointer par *cpt séparer par une tabulation, le caractère
//    tabulation, la chaine de caractères pointée par s et la fin de ligne.
//    Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec.
static int scptr_display_mult(const char *s, da *cpt);

//  rfree : libère la zone mémoire pointée par ptr et renvoie zéro.
static int rfree(void *ptr);


int main(int argc, char *argv[]){
  if (argc == 1){
    fprintf(stderr, "Illegal number of parameters or unrecognized option.\n");
    fprintf(stderr, "Syntaxe : %s [fichier] or  %s [fichier1] [fichier2] ...\n",
        argv[0], argv[0]);
    return EXIT_FAILURE;
  }
  int r = EXIT_SUCCESS;
  holdall *has = holdall_empty();
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *))strcmp,
      (size_t (*)(const void *))str_hashfun);
  if (has == NULL
      || ht == NULL) {
    goto error_capacity;
  }
  for (size_t k = 1; (int)k < argc; ++k){
    dsa *p = dsa_empty();
    if (p == NULL){
      goto error_capacity;
    }
    printf("%s\n", argv[1]);
    FILE *f = fopen(argv[k], "rb");
    if (f == NULL){
      goto error_file;
    }
    if (argc <= 2) {
      size_t c = 1;
      size_t *cpt = &c;
      int res;
      while ((res = dsa_add(p,f, cpt)) == 0){
        char *tmp[dsa_length_string(p)];
        char *s = malloc(dsa_length_string(p));
        if (s == NULL) {
          goto error_capacity;
        }
        for (size_t k = 0; k <= dsa_length_string(p); ++k){
          tmp[k] = dsa_ref_string(p, k);
        }
        strcpy(s, *tmp);
        da *cptr = hashtable_search(ht, s);
        if (cptr != NULL) {
          if (argc == 2) {
            if (dsa_add_cpt(p, cpt) == NULL){
              goto error_capacity;
            }
          } else {
            cptr = dsa_cpt(p);
            ++(*cpt);
            if (da_mod_ref(cptr, 0, cpt) == NULL){
              goto error_write;
            }
          }
        } else {
          if (holdall_put(has, s) != 0) {
            free(s);
            goto error_capacity;
          }
          cptr = dsa_cpt(p);
          if (hashtable_add(ht, s, cptr) == NULL) {
            goto error_capacity;
          }
        }
        if (argc == 2) {
          ++(*cpt);
        }
        dsa_dispose(&p);
        dsa *p = dsa_empty();
        if (p == NULL){
          goto error_capacity;
        }
      }
      if (r < 0){
        goto error_read;
      }
    } else {
      size_t c = 0;
      size_t *cpt = &c;
      int res;
      while ((res = dsa_add(p, f, cpt)) == 0){
        char *tmp[dsa_length_string(p)];
        for (size_t k = 0; k <= dsa_length_string(p); ++k){
          tmp[k] = dsa_ref_string(p, k);
        }
        char *s = malloc(dsa_length_string(p));
        if (s == NULL) {
          goto error_capacity;
        }
        strcpy(s, *tmp);
        da *cptr = hashtable_search(ht, s);
        if (cptr != NULL) {
          cptr = dsa_cpt(p);
          ++(*cpt);
          if (da_mod_ref(cptr, k - 2, cpt) == NULL){
            goto error_write;
          }
        }
        dsa_dispose(&p);
        dsa *p = dsa_empty();
        if (p == NULL){
          goto error_capacity;
        }
      }

      if (r < 0){
        goto error_read;
      }
    }
    if (fclose(f) < 0){
      goto error_read;
    }
  }
  if (argc == 2){
    if (holdall_apply_context(has,
        ht, (void *(*)(void *, void *))hashtable_search,
        (int (*)(void *, void *))scptr_display_one) != 0){
      goto error_write;
    }
  } else {
    if (holdall_apply_context(has, ht,
        (void *(*)(void *, void *))hashtable_search,
        (int (*)(void *, void *))scptr_display_mult) != 0){
      goto error_write;
    }
  }
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory\n");
  goto error;
error_file:
  fprintf(stderr, "*** Error: file doesn't exist\n");
  goto error;
error_read:
  fprintf(stderr, "*** Error: A read error occurs\n");
  goto error;
error_write:
  fprintf(stderr, "*** Error: A write error occurs\n");
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  hashtable_dispose(&ht);
  if (has != NULL) {
    holdall_apply(has, rfree);
  }
  holdall_dispose(&has);
  return r;
}

size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

static int scptr_display_one(const char *s, da *cpt) {
  if (da_length(cpt) < 2 ){
    return 0;
  }
  for (size_t k = 0; k <= da_length(cpt); ++k){
    fprintf(stdout, "%ls,", (int *)da_ref(cpt, k));
  }
  return fprintf(stdout, "\t%s\n", s) < 0;
}

static int scptr_display_mult(const char *s, da *cpt) {
  if (da_length(cpt) < 2){
    return 0;
  }
  for (size_t k = 0; k <= da_length(cpt); ++k){
    fprintf(stdout, "%ls\t", (int *)da_ref(cpt, k));
  }
  return fprintf(stdout, "%s\n", s) < 0;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

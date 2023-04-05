#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "da.h"
#include "holdall.h"
#include "hashtable.h"
#include "opt.h"

#define TRACK fprintf(stderr, "*** %s:%d\n", __func__, __LINE__);

//--- MACRO --------------------------------------------------------------------

#define DESC                                                                   \
  "Si un seul fichier en argument sur la ligne de commande, affiche pour "     \
  "chaque ligne de texte non vide apparaissant au moins deux fois dans le "    \
  "fichier, les numéros des lignes où elle apparaissent et le contenu de la "  \
  "ligne. "                                                                    \
  "Si au moins deux noms de fichiers en argument sur la ligne de commande, "   \
  "affiche pour chaque ligne de texte non vide apparaissant" \
  "au moins une fois dans tous les fichiers, le nombre d’occurrences de la "   \
  "ligne dans chacun des fichiers et le contenu de la ligne.\n"                \

#define USAGE "Syntaxe : %s [fichier] or  %s [fichier1] [fichier2] ...\n"

//--- Création des options -----------------------------------------------------

#define NBOPTION 2

//--- Gestion des arguments ----------------------------------------------------

typedef struct {
  int (*filter)(int c);
  int (*transform)(int c);
  da *filelist;
} cnxt;

//  str_hashfun : l'une des fonctions de pré-hachage conseillées par Kernighan
//    et Pike pour les chaines de caractères.
static size_t str_hashfun(const char *s);

//  scptr_display_one : affiche sur la sortie standard le tableau pointer par
// *cpt, le caractère
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

static int rdafree(da *p);

//  addline : On suppose que le fichier filename est ouvert en lecture. Tente
//    sinon de lire une ligne du filename et de l'ajouter au tableaux
//    dynamique pointer par p.
//    Renvoie une valeur négative en cas de dépassement de capacité ou de
//    probleme de lecture sur le fichier, renvoie zéro sinon;
static int addline(da *p, FILE *filename, cnxt *cntxt);

//  addfile : Ajoute le du nom du fichier filename au tableau dynamique pointer
//    par p.
//    Renvoie NULL en cas de dépassement de capacité, filename sinon.
static void *addfile(cnxt *p, const char *filename);

static int filter_choose(cnxt *cntxt, const char *s);

static int transform_choose(cnxt *cntxt, const char *s);

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Illegal number of parameters or unrecognized option.\n");
    printf(USAGE, argv[0], argv[0]);
    return EXIT_FAILURE;
  }
  opt *opt1 = opt_gen("-u", "--uppercase",
      "Met tous les caractéres enregistrer en majuscule", false,
      (int (*)(const void *, const void *))transform_choose);
  opt *opt2 = opt_gen("-f", "--filter=",
      "Applique le filtre passer en argument", true,
      (int (*)(const void *, const void *))filter_choose);
  opt *suppopt[NBOPTION] = {
    opt1, opt2
  };
  int r = EXIT_SUCCESS;
  da *filelist = da_empty();
  holdall *has = holdall_empty();
  holdall *hascpt = holdall_empty();
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *))strcmp,
      (size_t (*)(const void *))str_hashfun);
  if (has == NULL || ht == NULL || hascpt == NULL || filelist == NULL) {
    goto error_capacity;
  }
  cnxt cntxt = {
    .filelist = filelist, .filter = NULL, .transform = NULL
  };
  returnopt res;
  if ((res = opt_init(argc, argv, suppopt, NBOPTION, &cntxt,
      DESC, USAGE, (void *(*)(void *, const void *))addfile)) != SUCCESS) {
        TRACK
    if (res == HELP) {
      goto dispose;
    }
    if (res == ERR_ADD) {
      goto error_capacity;
    }
    if (res == ERR_OPT) {
      fprintf(stderr, "*** Error: Bad argument for option\n");
      goto error;
    }
    if (res == NO_OPT) {
      for (int k = 1; k < argc; ++k) {
        if (addfile(&cntxt, argv[k]) == NULL) {
          goto error_capacity;
        }
      }
    }
  }
  size_t len = da_length(cntxt.filelist);
  for (size_t k = 0; k < len; ++k) {
    fprintf(stderr, "%s\n", (char *)da_ref(cntxt.filelist, k));
  }
  for (size_t k = 0; k < len; ++k) {
    FILE *f = fopen(da_ref(cntxt.filelist, k), "rb");
    da *line = da_empty();
    da *cptt = da_empty();
    if (line == NULL || cptt == NULL || f == NULL) {
      if (f == NULL) {
        TRACK
        goto error_file;
      }
      goto error_capacity;
    }
    int nbline = 1;
    int resline;
    while ((resline = addline(line, f, &cntxt)) == 0) {
      if (da_length(line) != 0){
        char *s = malloc(da_length(line) + 1);
        if (s == NULL) {
          goto error_capacity;
        }
        for (size_t k = 0; k < da_length(line); ++k) {
          char *tmp = da_ref(line, k);
          s[k] = *tmp;
        }
        da *cptr = hashtable_search(ht, s);
        if (cptr != NULL) {
          if (k == 0) {
            if (len == 1) {
              int *cpt = malloc(sizeof *cpt);
              if (cpt == NULL) {
                goto error_capacity;
              }
              *cpt = nbline;
              if (da_add(cptr, cpt) == NULL) {
                goto error_capacity;
              }
            } else {
              int *cpt = da_ref(cptt, k);
              *cpt += 1;
            }
          } else {
            if (da_length(cptr) <= k + 1) {
              int *cpt = malloc(sizeof *cpt);
              if (cpt == NULL) {
                goto error_capacity;
              }
              *cpt = 1;
              if (da_add(cptr, cpt) == NULL) {
                goto error_capacity;
              }
              if (holdall_put(hascpt, cptr) != 0) {
                free(cptr);
                goto error_capacity;
              }
            } else {
              int *cpt = da_ref(cptt, k);
              (*cpt)++;
            }
          }
        } else {
          if (k == 0) {
            if (holdall_put(has, s) != 0) {
              free(s);
              goto error_capacity;
            }
            int *cpt = malloc(sizeof *cpt);
            if (cpt == NULL) {
              goto error_capacity;
            }
            if (len == 1){
              *cpt = nbline;
            } else {
              *cpt = 1;
            }
            if (da_add(cptt, cpt) == NULL) {
              goto error_capacity;
            }
            if (holdall_put(hascpt, cptt) != 0) {
              free(cptr);
              goto error_capacity;
            }
            if (hashtable_add(ht, s, cptt) == NULL) {
              goto error_capacity;
            }
          }
        }
        da_dispose(&line);
        line = da_empty();
        cptt = da_empty();
        if (line == NULL || cptt == NULL) {
          goto error_capacity;
        }
      }
      if (len == 1) {
        ++nbline;
      }
    }
    if (resline < 0) {
      goto error_read;
    }
    da_dispose(&line);
    da_dispose(&cptt);
    if (!feof(f)) {
      goto error_read;
    }
    if (fclose(f) != 0) {
      goto error_file;
    }
  }
  if (len == 1) {
    if (holdall_apply_context(has,
        ht, (void *(*)(void *, void *))hashtable_search,
        (int (*)(void *, void *))scptr_display_one) != 0) {
      goto error_write;
    }
  } else {
    if (holdall_apply_context(has,
        ht, (void *(*)(void *, void *))hashtable_search,
        (int (*)(void *, void *))scptr_display_mult) != 0) {
      goto error_write;
    }
  }
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory\n");
  goto error;
error_file:
  fprintf(stderr, "*** Error: An error on the file occurs\n");
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
if (suppopt != NULL) {
    for (int k = 0; k < NBOPTION; ++k){
      opt_dispose(&suppopt[k]);
    }
  }
  da_dispose(&(cntxt.filelist));
  hashtable_dispose(&ht);
  if (has != NULL) {
    holdall_apply(has, rfree);
  }
  if (hascpt != NULL) {
    holdall_apply(hascpt, (int (*) (void *))rdafree);
  }
  holdall_dispose(&has);
  holdall_dispose(&hascpt);
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
  if (da_length(cpt) < 2) {
    return 0;
  }
  for (size_t k = 0; k < da_length(cpt); k++) {
    int *c = (da_ref(cpt, k));
    printf("%d,", *c);
  }
  return printf("\t%s\n", s) < 0;
}

static int scptr_display_mult(const char *s, da *cpt) {
  if (da_length(cpt) < 2) {
    return 0;
  }
  for (size_t k = 0; k < da_length(cpt); k++) {
    printf("%ls\t", (int *) da_ref(cpt, k));
  }
  return printf("%s\n", s) < 0;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int rdafree(da *p){
  da_dispose(&p);
  return 0;
}

int addline(da *p, FILE *filename, cnxt *cntxt) {
  int c;
  while ((c = fgetc(filename)) != EOF && c != '\n') {
    if (cntxt->filter == NULL || cntxt->filter(c) != 0) {
      if (cntxt->transform == NULL || (c = cntxt->transform(c))) {
        char *s = malloc(sizeof *s);
        if (s == NULL) {
          return -1;
        }
        *s = (char) c;
        if (da_add(p, s) == NULL) {
          return -1;
        }
      }
    }
  }
  if (da_length(p) != 0){
    char *s = malloc(sizeof *s);
    if (s == NULL) {
      return -1;
    }
    c = '\0';
    *s = (char) c;
    if (da_add(p, s) == NULL) {
      return -1;
    }
  }
  if (ferror(filename) != 0) {
    return -1;
  }
  if (feof(filename)) {
    return 1;
  }
  return 0;
}

void *addfile(cnxt *p, const char *filename) {
  if (p->filelist== NULL) {
    return NULL;
  }
  if (da_add(p->filelist, filename) == NULL) {
    return NULL;
  }
  return (char *) filename;
}

//--- Fonction pour les option -------------------------------------------------

int transform_choose(cnxt *cntxt, const char *s) {
  if (strcmp("-u", s) == 0 || strcmp("--uppercase", s) == 0) {
    cntxt->transform = toupper;
    return 0;
  }
  return -1;
}

int filter_choose(cnxt *cntxt, const char *s) {
  if (strcmp("isalnum", s) == 0) {
    cntxt->filter = isalnum;
    return 0;
  }
  if (strcmp("isalpha", s) == 0) {
    cntxt->filter = isalpha;
    return 0;
  }
  if (strcmp("isblank", s) == 0) {
    cntxt->filter = isblank;
    return 0;
  }
  if (strcmp("iscntrl", s) == 0) {
    cntxt->filter = iscntrl;
    return 0;
  }
  if (strcmp("isdigit", s) == 0) {
    cntxt->filter = isdigit;
    return 0;
  }
  if (strcmp("isgraph", s) == 0) {
    cntxt->filter = isgraph;
    return 0;
  }
  if (strcmp("islower", s) == 0) {
    cntxt->filter = islower;
    return 0;
  }
  if (strcmp("isprint", s) == 0) {
    cntxt->filter = isprint;
    return 0;
  }
  if (strcmp("ispunct", s) == 0) {
    cntxt->filter = ispunct;
    return 0;
  }
  if (strcmp("isspace", s) == 0) {
    cntxt->filter = isspace;
    return 0;
  }
  if (strcmp("isupper", s) == 0) {
    cntxt->filter = isupper;
    return 0;
  }
  if (strcmp("isxdigit", s) == 0) {
    cntxt->filter = isxdigit;
    return 0;
  }
  return -1;
}

#include <stdio.h>
#include <string.h>
#include "da.h"
#include "holdall.h"
#include "hashtable.h"
#include "opt.h"

//--- MACRO --------------------------------------------------------------------

#define DESC                                                                   \
  "Si un seul fichier en argument sur la ligne de commande, affiche pour "     \
  "chaque ligne de texte non vide apparaissant au moins deux fois dans le "    \
  "fichier, les numéros des lignes où elle apparaissent et le contenu de la "  \
  "ligne. "                                                                    \
  "Si au moins deux noms de fichiers en argument sur la ligne de commande, "   \
  "affiche pour chaque ligne de texte non vide apparaissant au moins une fois " \
  "dans tous les fichiers, le nombre d’occurrences de la ligne dans chacun "   \
  "des fichiers et le contenu de la ligne."                                    \

#defini USAGE "Syntaxe : \lnid [fichier] or  \lnid [fichier1] [fichier2] ...\n"

//--- Création des options -----------------------------------------------------

#define SUPPOPT_LENGHT  2

#define SUPPOPT                                                                \
  opt_gen("-u", "--uppercase",                                                 \
    "Met tous les caractéres enregistrer en majuscule", false);                \
  opt_gen("-f", "--filter",                                                    \
    "Applique le filtre passer en argument", false);                           \

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

//  addline : On suppose que le fichier filename est ouvert en lecture. Tente
//    sinon de lire une ligne du filename et de l'ajouter au tableaux
//    dynamique pointer par p.
//    Renvoie une valeur négative en cas de dépassement de capacité ou de
//    probleme de lecture sur le fichier, renvoie zéro sinon;
int addline(da *p, FILE *filename);

//  addfile : Ajoute le du nom du fichier filename au tableau dynamique pointer
//    par p.
//    Renvoie une valeur négative en cas d'erreur, zéro sinon.
int addfile(da *p, const char *filename);

int main(int argc, char **argv[]) {
  if (argc == 1) {
    fprintf(stderr, "Illegal number of parameters or unrecognized option.\n");
    printf("%s", USAGE);
    return EXIT_FAILURE;
  }
  opt *suppopt[] = {
    SUPPOPT;
  }
  int r = EXIT_SUCCESS;
  da *filelist = da_empty();
  holdall *has = holdall_empty();
  holdall *hascpt = holdall_empty();
  hashtable *ht = hashtable_empty((int (*)(const void *, const void *))strcmp,
      (size_t (*)(const void *))str_hashfun);
  if (has == NULL || ht == NULL || hascpt == NULL || filelist ||) {
    goto error_capacity;
  }
  cxnt cntxt {
    .filelist = filelist, filter = NULL, transform = NULL
  };
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
  if (da_length(cpt) < 2) {
    return 0;
  }
  for (size_t k = 0; k <= da_length(cpt); ++k) {
    fprintf(stdout, "%ls,", (int *) da_ref(cpt, k));
  }
  return fprintf(stdout, "\t%s\n", s) < 0;
}

static int scptr_display_mult(const char *s, da *cpt) {
  if (da_length(cpt) < 2) {
    return 0;
  }
  for (size_t k = 0; k <= da_length(cpt); ++k) {
    fprintf(stdout, "%ls\t", (int *) da_ref(cpt, k));
  }
  return fprintf(stdout, "%s\n", s) < 0;
}

int rfree(void *ptr) {
  free(ptr);
  return 0;
}

int addline(da *p, FILE *filename) {
  int c;
  while ((c = fgetc(filename)) != EOF || c != '\n') {
    char *s = malloc(sizeof *s);
    if (s == NULL) {
      return -1;
    }
    *s = (char) c;
    if (da_add(p, s) == NULL) {
      return -1;
    }
  }
  char *s = malloc(sizeof *s);
  if (s == NULL) {
    return -1;
  }
  c = '\0';
  *s = (char) c;
  if (da_add(p, s) == NULL) {
    return -1;
  }
  if (ferror(filename) != 0) {
    return -1;
  }
  if (c == '\n' && feof(filename) == 0) {
    return 1;
  }
  return 0;
}

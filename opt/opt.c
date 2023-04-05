// Partie implémentation d'un module pour la gestion d'otion (opt)

#include "opt.h"
#define TRACK fprintf(stderr, "*** %s:%d\n", __func__, __LINE__);

//--- Définition opt -----------------------------------------------------------
struct opt {
  const char *shortopt;
  const char *longopt;
  const char *desc;
  bool arg;
  int (*fun) (const void *,const void *);
};


//--- Fonction interne ---------------------------------------------------------

//  est_prefixe : Renvoie NULL si s1 n'est pas un préfixe de s2, sinon renvoie
//    le reste de s2 qui n'est pas dans s1.
static const char *suffixe(const char *s1, const char *s2) {
  if (*s1 == '\0'){
    return s2;
  }
  if (*s2 == '\0' || *s1 != *s2){
    return NULL;
  }
  return suffixe (s1 + 1, s2 + 1);
}

const char *opt_parse(opt *optn, const char *argv, char *s) {
  int i = 0;
  while (optn->longopt[i] == argv[i]) {
    s[i] = argv[i];
    i++;
  }
  return s;
}

typedef enum {
  NOT_OPT,
  ERR_ADDTEST,
  SHORTOPT,
  SHORTOPTARG,
  LONGOPT,
} returntest;


static returntest opt_test(void * cntxt, opt **optsupp, const char **argv, int k,
  size_t nbopt) {
  for (size_t i = 0; i < nbopt; ++i){
    if (strcmp((optsupp[i])->shortopt, argv[k]) == 0) {
      if (optsupp[i]->arg){
        if ((optsupp[i]->fun (cntxt, argv[k + 1]))  != 0){
          return ERR_ADDTEST;
        }
        return SHORTOPTARG;
      } else {
        if ((optsupp[i]->fun (cntxt, argv[k]))  != 0){
          return ERR_ADDTEST;
        }
        return SHORTOPT;
      }
    }
    char *s = malloc(strlen((optsupp[i])->longopt));
    if (strcmp((optsupp[i])->longopt, opt_parse(optsupp[i], argv[k], s)) == 0) {
      if (optsupp[i]->arg){
        if ((optsupp[i]->fun (cntxt, (suffixe(optsupp[i]->longopt, argv[k])))) != 0){
          return ERR_ADDTEST;
        }
      } else {
        if ((optsupp[i]->fun (cntxt, argv[k])) != 0){
          return ERR_ADDTEST;
        }
      }
      return LONGOPT;
    }
    free(s);
  }
  return NOT_OPT;
}

//--- Fonctions opt ------------------------------------------------------------



opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
  bool arg, int (*fun) (const void *,const void *)) {
  opt *op = malloc (sizeof *op);
  if (op == NULL){
    return NULL;
  }
  op->shortopt = shortopt;
  op->longopt = longopt;
  op->desc = desc;
  op->arg = arg;
  op->fun = fun;
  return op;
}

void opt_dispose (opt **aopt){
  if (*aopt == NULL) {
    return;
  }
  free (*aopt);
}

#define PRINTF_OPT(opta) printf("\t%s%s or %s%s :\t%s\n", opta->shortopt, \
  (opta->arg ? " [option]" : ""), opta->longopt,                               \
  (opta->arg ? "[option]" : ""), opta->desc);                                 \

#define SHORTHELP "-h"
#define LONGHELP "--help"


returnopt opt_init(int argc, const char **argv, opt **optsupp, size_t nbopt,
  void *cntxt, const char *desc, const char *usage,
  void *(*fun) (void *, const void *)){
  if (argc < 2){
    return NO_OPT;
  }
  for (int k = 1; k < argc; ++k){
    if (strcmp(SHORTHELP, argv[k]) == 0 || strcmp(LONGHELP, argv[k]) == 0){
      if (desc != NULL){
        printf("%s\n", desc);
      }
      if (usage != NULL){
        printf("%s", usage);
      }
      for (size_t i = 0; i < nbopt; ++i){
        PRINTF_OPT(optsupp[i]);
      }
      return HELP;
    }
    int res;
    if ((res = opt_test(cntxt, optsupp, argv, k, nbopt)) != SHORTOPT
      || res != LONGOPT){
      if (res == NOT_OPT) {
        if (fun(cntxt, argv[k]) == NULL){
          return ERR_ADD;
        }
      } else if (res == ERR_ADDTEST) {
        return ERR_OPT;
      }
    }

    if (res == SHORTOPTARG) {
      k++;
    }
  }
  return SUCCESS;
}

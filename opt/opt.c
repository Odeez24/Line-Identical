// Partie implémentation d'un module pour la gestion d'otion (opt)

#include "opt.h"


//--- Définition opt -----------------------------------------------------------
struct opt {
  const char *shortopt;
  const char *longopt;
  const char *desc;
  bool arg;
  int (*fun) (const void *,const void *);
};


//--- Fonction interne ---------------------------------------------------------

//  est_prefixe : Test si la chaîne de caractére pointer par s1 est un prefixe
//    de celle pointer par s2.
//    Renvoie NULL si s1 n'est pas un préfixe de s2, sinon renvoie le reste de
//    s2 qui n'est pas dans s1
static const char * est_prefixe(const char *s1, const char *s2) {
  if (*s1 == '\0'){
    return s2;
  }
  if (*s2 == '\0' || *s1 != *s2){
    return NULL;
  }
  return est_prefixe(s1 + 1, s2 + 1);
}

const char *opt_select(opt *optsupp, char **argv, int k) {
  if (strcmp(optsupp->shortopt, argv[k]) == 0 && optsupp->arg){
    return argv[k + 1];
  } else if (strcmp(optsupp->longopt, argv[k]) == 0 && optsupp->arg){
    return est_prefixe(optsupp->longopt, argv[k]);
  }
  return argv[k];
}


static int opt_test(void * cntxt, opt **optsupp, char **argv, int k,
  size_t nbopt) {
  size_t nb = 0;
  for (size_t i = 0; i < nbopt; ++i){
    if (strcmp((optsupp[i])->shortopt, argv[k]) == 0
      || strcmp((optsupp[i])->longopt, argv[k]) == 0) {
      if ((optsupp[i]->fun (cntxt, (opt_select(optsupp[i], argv, k)))) != 0){
        return -1;
      }
    } else {
      ++nb;
    }
  }
  if (nb == nbopt) {
    return 1;
  }
  return 0;
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

#define PRINTF_OPT(opta) printf("\t%s%s\t or %s%s : \n\t%s\n", opta->shortopt, \
  (opta->arg ? " [option]" : ""), opta->longopt,                               \
  (opta->arg ? "[option]" : ""), opta->desc);                                 \

#define SHORTHELP "-h"
#define LONGHELP "--help"


returnopt opt_init(int argc, char **argv, opt **optsupp, size_t nbopt,
  void *cntxt, const char *desc, const char *usage,
  void *(*fun) (void *, const void *)){
  if (argc <= 2){
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
    if ((res = opt_test(cntxt, optsupp, argv, k, nbopt)) != 0){
      if (res > 0) {
        if (fun(cntxt, argv[k]) == NULL){
          return ERR_ADD;
        }
      } else if (res < 0) {
        return ERR_OPT;
      }
    }
  }
  return SUCCESS;
}

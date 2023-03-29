// Partie implémentation d'un module pour la gestion d'otion (opt)

#include "opt.h"


//--- Définition opt -----------------------------------------------------------
struct opt {
  const char *shortopt;
  const char *longopt;
  const char *desc;
  bool arg;
  int (*fun) (void *);
};

//--- Raccourcis opt -----------------------------------------------------------
#define SHORT(op)   ((op)->shortopt);
#define LONG(op)    ((op)->longopt);
#define DESC(op)    ((op)->desc);
#define ARG(op)     ((op)->arg);

//--- Fonction interne ---------------------------------------------------------

//  est_prefixe : Test si la chaîne de caractére pointer par s1 est un prefixe
//    de celle pointer par s2.
static bool est_prefixe(const char *s1, const char *s2) {
  if (*s1 == '\0'){
    return true;
  }
  if (*s2 == '\0' || *s1 != *s2){
    return false;
  }
  return est_prefixe(s1 + 1, s2 + 1);
}

static int opt_test(*funopt, opt **optsupp, size_t nbopt) {


//--- Fonctions opt ------------------------------------------------------------



opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
  bool arg) {
  opt *op = malloc (sizeof *op);
  if (op == NULL){
    return NULL;
  }
  op->shortopt = shortopt;
  op->longopt = longopt;
  op->desc = desc;
  op->arg = arg;
  return op;
}

#define PRINTF_OPT(opta) printf("\t%s%s\t or %s%s : \n\t%s\n", opta->shortopt, \
  (opta->arg ? " [option]" : ""), opta->longopt,                               \
  (opta->arg ? "=[option]" : ""), opta->desc);                                 \

#define SHORTHELP "-h"
#define LONGHELP "--help"


returnopt opt_parse(int argc, const char **argv, opt **optsupp, size_t nbopt,
  void *cntxt, const char *desc, const char *usage,
  void * (*fun) (void *, void *)){
  if (argc <= 2){
    return NO_OPT;
  }
  for (int k = 1; k < argc; ++k){
    if (strcmp(SHORTHELP, argv[k]) == 0 || strcmp(LONGHELP, argv[k]) == 0){
      if (desc != NULL){
        printf("%s\n", desc);
      }
      if (usage != NULL){
        printf("%s\n", usage);
      }
      for (size_t i = 0; i < nbopt; ++i){
        PRINTF_OPT(optsupp[i]);
      }
      return HELP;
    }
    for (size_t i = 0; i < nbopt) {
      if (strcmp(optsupp[i], argv[k]) == 0
        || strcmp([optsupp[i], argv[k]) == 0) {
      cntxt->

  }
}

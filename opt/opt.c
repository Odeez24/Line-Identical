// Partie implémentation d'un module pour la gestion d'otion (opt)

#include "opt.h"


//--- Définition opt -----------------------------------------------------------
struct opt {
  const char *shortopt;
  const char *longopt;
  const char *desc;
  bool arg;
};

//--- Raccourcis opt -----------------------------------------------------------
#define SHORT(op)   ((op)->shortopt);
#define LONG(op)    ((op)->longopt);
#define DESC(op)    ((op)->desc);
#define ARG(op)     ((op)->arg);

//--- Fonction interne ---------------------------------------------------------

//  est_prefixe : Test si la chaîne de caractére pointer par s1 est un prefixe
//    de celle pointer par s2.
bool est_prefixe(const char *s1, const char *s2){
  if (s1 == '\0'){
    return true;
  }
  if (s2 == '\0' || *s1 != *s2){
    return false;
  }
  return est_prefixe(s1 + 1, s2 + 1);
}

//--- Fonctions opt ------------------------------------------------------------

opt *opt_gen(const char *shortopt, const char *longopt, const char *desc, bool arg){
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



returnopt opt_parse(int argc, const char *argv, char *optsupp, char *other,
              char *optlist){
  if (argc <= 2){
    return NO_OPT;
  }

// Partie implémentation d'un module pour la gestion d'otion (opt)

#include "opt.h"

//--- Définition opt -----------------------------------------------------------
struct opt {
  const char *shortopt;
  const char *longopt;
  const char *desc;
  bool arg;
  int (*fun)(const void *, const void *);
};

//--- Fonction interne ---------------------------------------------------------

//  suffixe : Renvoie NULL si s1 n'est pas un préfixe de s2, sinon renvoie
//    le reste de s2 qui n'est pas dans s1.
static const char *suffixe(const char *s1, const char *s2) {
  if (*s1 == '\0') {
    return s2;
  }
  if (*s2 == '\0' || *s1 != *s2) {
    return NULL;
  }
  return suffixe(s1 + 1, s2 + 1);
}

//  prefix : Affecte a s la partie commune entre les chaînes de caractére s1 et
//    s2.
//  Renvoie NULL si les deux chaînes n'ont rien en commun, s sinon.
void *prefix(const char *s1, const char *s2, char *s) {
  int i = 0;
  while (s1[i] == s2[i] && s1[i] != '\0') {
    s[i] = s2[i];
    i++;
  }
  s[i] = '\0';
  if (i == 0) {
    return NULL;
  }
  return s;
}

//  returntest : Enumération des différente valeur de retour de la fonction
//    opt_test.
typedef enum {
  NOT_OPT,
  ERR_ADDTEST,
  CAP_ERR,
  SHORTOPT,
  SHORTOPTARG,
  LONGOPT,
} returntest;

//  opt_test : Test si l'argument d'indice k contenu dans argv est une option
//    contenu dans le tableau optsupp. Si oui alors appelle sa fonction
//    d'initialisation.
//  Renvoie NOT_OPT si l'argument n'est pas une option, ERR_ADDTEST si une
//    erreur est survenu lors de l'exécution de la fonction d'initialisation
//    d'une option, CAP_ERR en cas de dépassement de capacité. SHORTOPT si
//    l'argument est la version courte de l'option, SHORTOPTARG si l'argument
//    est la version courte de l'option et que l'option prend un argument ou
//    LONGOPT si l'argument est la version longue de l'option.
static returntest opt_test(void *cntxt, opt **optsupp, const char **argv, int k,
    size_t nbopt) {
  for (size_t i = 0; i < nbopt; ++i) {
    if (strcmp((optsupp[i])->shortopt, argv[k]) == 0) {
      if (optsupp[i]->arg) {
        if ((optsupp[i]->fun(cntxt, argv[k + 1])) != 0) {
          return ERR_ADDTEST;
        }
        return SHORTOPTARG;
      } else {
        if ((optsupp[i]->fun(cntxt, argv[k])) != 0) {
          return ERR_ADDTEST;
        }
        return SHORTOPT;
      }
    }
    char s[strlen((optsupp[i])->longopt)];
    if (prefix(optsupp[i]->longopt, argv[k], s) != NULL) {
      if (strcmp((optsupp[i])->longopt, s) == 0) {
        if (optsupp[i]->arg) {
          if ((optsupp[i]->fun(cntxt,
              (suffixe(optsupp[i]->longopt, argv[k])))) != 0) {
            return ERR_ADDTEST;
          }
        } else {
          if ((optsupp[i]->fun(cntxt, argv[k])) != 0) {
            return ERR_ADDTEST;
          }
        }
        return LONGOPT;
      }
    }
  }
  return NOT_OPT;
}

//--- Fonctions opt ------------------------------------------------------------

opt *opt_gen(const char *shortopt, const char *longopt, const char *desc,
    bool arg, int (*fun)(const void *, const void *)) {
  opt *op = malloc(sizeof *op);
  if (op == NULL) {
    return NULL;
  }
  op->shortopt = shortopt;
  op->longopt = longopt;
  op->desc = desc;
  op->arg = arg;
  op->fun = fun;
  return op;
}

void opt_dispose(opt **aopt) {
  if (*aopt == NULL) {
    return;
  }
  free(*aopt);
}

#define PRINTF_OPT(opta) printf("\t%s%s or %s%s :\t%s\n", opta->shortopt,      \
    (opta->arg ? " [option]" : ""), opta->longopt,                             \
    (opta->arg ? "[option]" : ""), opta->desc);                                \

#define SHORTHELP "-h"
#define LONGHELP "--help"

returnopt opt_init(int argc, const char **argv, opt **optsupp, size_t nbopt,
    void *cntxt, const char *desc, const char *usage,
    void *(*fun)(void *, const void *)) {
  if (argc < 2) {
    return NO_OPT;
  }
  for (int k = 1; k < argc; ++k) {
    if (strcmp(SHORTHELP, argv[k]) == 0 || strcmp(LONGHELP, argv[k]) == 0) {
      if (desc != NULL) {
        printf("%s\n", desc);
      }
      if (usage != NULL) {
        printf("%s", usage);
      }
      for (size_t i = 0; i < nbopt; ++i) {
        PRINTF_OPT(optsupp[i]);
      }
      return HELP;
    }
    int res;
    if ((res = opt_test(cntxt, optsupp, argv, k, nbopt)) != SHORTOPT
        || res != LONGOPT) {
      if (res == NOT_OPT) {
        if (fun(cntxt, argv[k]) == NULL) {
          return ERR_ADD;
        }
      } else if (res == ERR_ADDTEST) {
        return ERR_OPT;
      } else if (res == CAP_ERR) {
        return ERROR;
      }
    }
    if (res == SHORTOPTARG) {
      k++;
    }
  }
  return SUCCESS;
}

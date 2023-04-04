#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "da.h"

#define TRACK fprintf(stderr, "*** %s:%d\n", __func__, __LINE__);

int addline(da *p, FILE *filename) {
  int c;
  while ((c = fgetc(filename)) != EOF && c != '\n') {
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
  if (feof(filename)) {
    return 1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 2){
    return EXIT_FAILURE;
  }
  int r = EXIT_SUCCESS;
  da *p = da_empty();
  if (p == NULL) {
    return EXIT_FAILURE;
  }
  FILE *f = fopen(argv[1], "rb");
  if (f == NULL){
    goto error_read;
  }
  while (addline(p, f) == 0) {
    char *tmp[da_length(p)];
    char *s = malloc(sizeof *s * (da_length(p)));
    if (s == NULL) {
      goto error_capacity;
    }
    for (size_t k = 0; k < da_length(p); k++) {
      tmp[k] = da_ref(p, k);
    }
    TRACK
    strcpy(s, *tmp);
    printf("%s\n", s);
    TRACK
     da_dispose(&p);
    TRACK
      p = da_empty();
      if (p == NULL) {
        goto error_capacity;
      }
  }
  fclose(f);
  size_t len = da_length(p);
  fprintf(stderr, "--- Info: Number of read character (da): %zu\n",
      len);
  if (len != 0) {
    for (size_t i = 0; i < len; ++i) {
      char *s = da_ref(p, i);
      printf("%c", *s);
    }
    printf("\n");
  }
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory\n");
  goto error;
error_read:
  fprintf(stderr, "*** Error: A read error occurs\n");
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  da_dispose(&p);
  return r;
}

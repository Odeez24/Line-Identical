#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "da.h"

int main(void) {
  int r = EXIT_SUCCESS;
  da *p = da_empty();
  if (p == NULL) {
    return EXIT_FAILURE;
  }
  char w;
  while (scanf("%c", &w) == 1) {
    char *s = malloc(sizeof *s);
    if (s == NULL) {
      goto capacity_error;
    }
    memcpy(s, &w, sizeof(w));
    if (da_add(p, s) == NULL) {
      goto capacity_error;
    }
  }
  if (!feof(stdin)) {
    goto error_read;
  }
  size_t len = da_length(p);
  fprintf(stderr, "--- Info: Number of read character (da): %zu\n",
      len);
  if (len != 0) {
    for (size_t i = 0; i < len; ++i) {
      char *s = da_ref(p, i);
      printf("- %c", *s);
    }
    printf("\n");
  }
  goto dispose;
capacity_error:
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

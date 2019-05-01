#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/struct/vector3.h"

static char **str_split(char *a_str, const char a_delim) {
  char **result = 0;
  size_t count = 0;
  char *tmp = a_str;
  char *last_comma = 0;
  char delim[2];
  delim[0] = a_delim;
  delim[1] = 0;

  /* Count how many elements will be extracted. */
  while (*tmp) {
    if (a_delim == *tmp) {
      count++;
      last_comma = tmp;
    }
    tmp++;
  }

  /* Add space for trailing token. */
  count += last_comma < (a_str + strlen(a_str) - 1);

  /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
  count++;

  result = malloc(sizeof(char *) * count);

  if (result) {
    size_t idx = 0;
    char *token = strtok(a_str, delim);

    while (token) {
      assert(idx < count);
      *(result + idx++) = strdup(token);
      token = strtok(0, delim);
    }
    assert(idx == count - 1);
    *(result + idx) = 0;
  }

  return result;
}

char *struct2str(int id, vector3 dir) {
  /* get lenght of string required to hold struct values */
  size_t len = 0;
  len = snprintf(NULL, len, "%d,%.02f,%.02f,%.02f", id, dir.x, dir.y, dir.z);

  /* allocate/validate string to hold all values (+1 to null-terminate) */
  char *apstr = calloc(1, sizeof *apstr * len + 1);
  if (!apstr) {
    fprintf(stderr, "%s() error: virtual memory allocation failed.\n",
            __func__);
  }

  /* write/validate struct values to apstr */
  if (snprintf(apstr, len + 1, "%d,%.02f,%.02f,%.02f", id, dir.x, dir.y,
               dir.z) > len + 1) {
    fprintf(stderr, "%s() error: snprintf returned truncated result.\n",
            __func__);
    return NULL;
  }

  return apstr;
}

vector3 str2stuct(char *str) {
  char **res = str_split(str, ',');
  vector3 r = {atof(res[1]), atof(res[2]), atof(res[3])};
  return r;
}

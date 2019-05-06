#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
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

void str2stuct(char *str, float* r) {
        char **res = str_split(str, ',');
        int id = atoi(res[0]);
        if(id == 1) {
                r[0] = 1;
                r[1] = atof(res[1]); r[2] = atof(res[2]); r[3] = atof(res[3]);
        } else if(id == 2) {
                r[0] = 2;
                r[1] = atof(res[1]); r[2] = atof(res[2]); r[3] = atof(res[3]);
                r[4] = atof(res[4]); r[5] = atof(res[5]); r[6] = atof(res[6]);
        }
        free(res);
}

char *struct2str(int id, float from[3], float to[3]) {
        char buff[0];

        /* get lenght of string required to hold struct values */
        int len1 = 0;
        if(id == 1) {
                len1 = snprintf(buff, len1, "%d,%.02f,%.02f,%.02f", id, from[0], from[1], from[2]);
        } else if(id == 2) {
                len1 = snprintf(buff, len1, "%d,%.02f,%.02f,%.02f,%.02f,%.02f,%.02f", id, from[0], from[1], from[2], to[0], to[1], to[2]);
        }

        /* allocate/validate string to hold all values (+1 to null-terminate) */
        char *apstr = calloc(1, sizeof *apstr * len1 + 1);
        if (!apstr) {
                fprintf(stderr, "%s() error: virtual memory allocation failed.\n", __func__);
        }

        /* write/validate struct values to apstr */
        int len2 = 0;
        if(id == 1) {
                len2 = snprintf(apstr, len1 + 1, "%d,%.02f,%.02f,%.02f", id, from[0], from[1], from[2]);
        } else if(id == 2) {
                len2 = snprintf(apstr, len1 + 1, "%d,%.02f,%.02f,%.02f,%.02f,%.02f,%.02f", id, from[0], from[1], from[2], to[0], to[1], to[2]);
        }

        if (len2 > len1 + 1) {
                fprintf(stderr, "%s() error: snprintf returned truncated result.\n", __func__);
                free(apstr);
                return NULL;
        }

        return apstr;
}

/* Petit plus */

void sdl2gl(float mouse[2]) {
        mouse[0] = (mouse[0] / _wW) * 2.0f - 1.0f;
        mouse[1] = 1.0f - (mouse[1] / _wH) * 2.0f;
}

float rad2deg(float rad) {
        return rad * 180 / M_PI;
}

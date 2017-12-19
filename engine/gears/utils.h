#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>

int32_t parse_int(const char *str, bool &ok);
int32_t parse_int(const char *str, bool &ok, int min, int max);

#endif // UTILS_H

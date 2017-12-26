#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>

int32_t parse_int(const char *str, bool &ok);
int32_t parse_int(const char *str, bool &ok, int min, int max);
float parse_float(const char *str, bool &ok);
float parse_float(const char *str, bool &ok, int min, int max);

void __attribute__((noinline)) breakAnchor(const char *name);

#endif // UTILS_H

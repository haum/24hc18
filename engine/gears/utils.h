#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdlib.h>

/** Get random angle in 0..2pi range **/
double random_angle();

/** Get random number in 0..1 range **/
double random_unit();

int32_t parse_int(const char *str, bool &ok);
int32_t parse_int(const char *str, bool &ok, int min, int max);
float parse_float(const char *str, bool &ok);
float parse_float(const char *str, bool &ok, float min, float max);

void __attribute__((noinline)) breakAnchor(const char *name);

#endif // UTILS_H

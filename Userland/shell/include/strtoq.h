#ifndef STRTOQ_H
#define STRTOQ_H
#include <stdint.h>
int64_t
strtoq(const char *nptr, char **endptr, register int base);

int64_t
strtouq(const char *nptr, char **endptr, register int base);
#endif

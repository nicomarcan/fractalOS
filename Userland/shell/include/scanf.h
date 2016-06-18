#ifndef SCANF_H
#define SCANF_H
#include <stdarg.h>
int sscanf(const char *ibuf, const char *fmt, ...);
int sscanf(const char *inp, const char *fmt, ...);
int vsscanf(const char *inp, char const *fmt0, va_list ap);
#endif

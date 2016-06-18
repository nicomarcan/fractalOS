#ifndef C_STRING_H
#define C_STRING_H
#include <stdint.h>
int64_t c_strcmp(const uint8_t * a, const uint8_t * b);
uint8_t c_isdigit(uint8_t c);
uint8_t c_isalpha(uint8_t c);
uint8_t c_isblank(uint8_t c);
uint8_t c_isspace(uint8_t c);
uint8_t c_islower(uint8_t c);
uint8_t c_isupper(uint8_t c);
uint8_t c_tolower(uint8_t c);
uint8_t c_toupper(uint8_t c);
uint8_t * c_strcpy ( uint8_t * destination, const uint8_t * source );
uint64_t c_strlen(const uint8_t * str);
#endif

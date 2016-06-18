#include <c_string.h>

int64_t c_strcmp(const uint8_t * a, const uint8_t * b) {
  while (*a == *b && *a != '\0') {
    a++;
    b++;
  }
  return *a - *b;
}

uint8_t c_isdigit(uint8_t c) {
  return (c >= '0' && c <= '9');
}

uint8_t c_isalpha(uint8_t c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

uint8_t c_isblank(uint8_t c) {
  return (c == '\n' || c == ' ' || c == '\t');
}

uint8_t c_tolower(uint8_t c) {
  if (c >= 'A' && c <= 'Z') {
    c = c - 'A' + 'a';
  }
  return c;
}

uint8_t c_toupper(uint8_t c) {
  if (c >= 'a' && c <= 'z') {
    c = c - 'a' + 'A';
  }
  return c;
}


uint8_t * c_strcpy ( uint8_t * destination, const uint8_t * source ) {
  uint8_t * retp = destination;
  do{
    *destination++ = *source;
  } while(*source++ != '\0');
  return retp;
}

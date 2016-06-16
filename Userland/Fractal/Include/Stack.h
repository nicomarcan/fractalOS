#ifndef LIST_H
#define LIST_H
#include <stdint.h>
/*
 * A stack that stores
 * pointers to values
 */
typedef struct {
  void * data;
  Node * next;
} Node;

typedef struct {
  Node * first ;
  uint32_t size ;
} Stack;

Stack * newStack();
void push(Stack * s,void * data);
void * pop(Stack * s);
uint8_t isEmpty(Stack *s);}
uint32_t size(Stack *s);

#endif

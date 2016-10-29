#ifndef LIST_H
#define LIST_H
#include <stdint.h>
/*
 * A stack that stores
 * pointers to values
 */
typedef struct Node {
  void * data;
  struct Node * next;
} Node;

typedef struct Stack {
  Node * first ;
  uint32_t size ;
} Stack;



Stack * newStack();
void push(Stack * s,void * data);
void * pop(Stack * s);
uint8_t isEmpty(Stack *s);
uint32_t size(Stack *s);
void apply(Stack * s,void p(void*));

#endif

#include <stack.h>
#include <stdlib.h>

Stack * newStack(){
  Stack * ans = malloc(sizeof(Stack));
  ans->data=ans->size=0;
  return ans ;
}

void push(Stack * s,void * data){
  Node * n = malloc(sizeof(Node));
  n->next=s->first;
  n->data=data;
  s->first=n;
  s->size++;
}

void * pop(Stack * s){
  if(isEmpty(s)){
    return 0;
  }
  void * ans = s->first->data;
  s->first=s->first->next;
  s->size--;
  return ans;
}

uint8_t isEmpty(Stack * s){
  return size(s)==0;
}

uint32_t size(Stack * s){
  return s->size;
}

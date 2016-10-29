#include <Stack.h>
#include <lib.h>
#define NULL 0
Stack * newStack(){
  Stack * ans = malloc(sizeof(Stack));
  ans->first=ans->size=0;
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
  Node * n = s->first;
  void * ans = n->data;
  s->first=s->first->next;
  s->size--;
  free(n);
  return ans;
}

uint8_t isEmpty(Stack * s){
  return size(s)==0;
}

uint32_t size(Stack * s){
  return s->size;
}

void apply(Stack * s,void p(void*)){
	apply_r(s->first,p);
	return;
}

void apply_r(Node *n,void p(void*)){
	if(n == NULL) return;
	p(n->data);
	apply_r(n->next,p);
	return;
}

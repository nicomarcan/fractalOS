#ifndef PRODCONSGUI_H
#define PRODCONSGUI_H

typedef struct guiprodcon guiprodcon;
guiprodcon * renderinit(uint64_t max_size);
void renderenque(guiprodcon * gs);
void renderdeque(guiprodcon * gs);

#endif

/* text mode video driver*/
#include <tm_driver.h>

#define SCREEN_WIDTH  80
#define SCREEN_HEIGHT 25
static char * vbuff    = (char *) 0xB8000;
static char * vbuffend = (char *) 0xB8FA0;

/*
 * moves cursor to next line.
 */
void new_line(char ** vptr) {
  *vptr = (char *) ((((int)*vptr + SCREEN_WIDTH) /  SCREEN_WIDTH) * SCREEN_WIDTH);
}

/*
 * deletes pointed char and backs up the video pointer
 */
void delete(char ** vptr) {
  *--(*vptr) = ' ';
}

/*
 * prints line with specified length
 */
void print_text(const char * sptr, char ** vptr, char color, char length){
    while(length--){
      if (*vptr >= vbuffend){
          *vptr = vbuff;
          clear();
      }
      switch(*sptr) {
        case '\b':
          delete(vptr);
          break;
        case '\n':
          new_line(vptr);
          break;
        default:
          *(*vptr)++ = *sptr++;
          break;
      }
      *(*vptr)++ = color;
    }
}

void clear() {
  char * vptr = vbuff;
  for (int i = 0; i <SCREEN_HEIGHT; i++) {
    for (int j = 0; j < SCREEN_WIDTH; j++) {
      *vptr++ = ' ';
    }
  }
}

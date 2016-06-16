#include <c_syscall.h>
#include <clib.h>

int64_t fread(uint64_t fd, uint8_t * buf, int64_t len);

void putchar(uint8_t c){
  write(STDOUT, &c, 1);
}

void putchars(uint8_t * str, uint64_t c) {
  write(STDOUT, str, c);
}

int64_t getchar(void) {
  static uint8_t buf[BUFSIZ];
  static uint8_t *bufp = buf;
  static int64_t n = 0;

  if (n == 0) {
    n = fread(STDIN, buf, sizeof (buf));
    bufp = buf;
  }
  return (--n >= 0) ? (uint8_t) *bufp++ : -1;
}

int64_t fread(uint64_t fd, uint8_t * buf, int64_t len) {
  int64_t ret = 0;
  int64_t count = 0;
  int64_t j = 0;
  uint8_t * bufp = buf;
  uint8_t ent_bool = 0;
  ncPrintDec(len);
  while (!ent_bool) {
    if (0 < (ret = read(fd, bufp, len - j))) {
      for (int i = 0; i < ret; i++) {
        if (bufp[i] == '\n') {
          buf[j] = '\n';
          putchar('\n');
          ent_bool = 1;
          j++;
        }
        else if (bufp[i] == '\b') {
          if (j > 0) {
            buf[j] = ' ';
            j--;
            putchar('\b');
          }
        }
        else {
          if (j < len - 1) {
            buf[j] = bufp[i];
            putchar(buf[j]);
            j++;
          }
        }
      }
    }
    bufp = buf + j;
  }
  putchars(buf, j);
  return j;
}

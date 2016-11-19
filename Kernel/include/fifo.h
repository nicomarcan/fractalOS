#ifndef FIFO_H
#define FIFO_H

#define BUFF_SIZE 100
#define ADDR_SIZE 20
#define FIFOS_MAX 100
#define WRITE 1
#define READ 2
typedef struct{
        int size;
	 char  * fifos[10];

} OPENED_FIFOS;

int64_t mkfifo(const char * addr);
int64_t rmfifo(const char * addr);
int64_t write_fifo(uint64_t fd ,const uint8_t * buf, uint64_t count );
int64_t read_fifo(uint64_t fd, uint8_t * buf, uint64_t count);
int64_t open_fifo(const char * addr,uint64_t mode);
void get_opened_fifos(OPENED_FIFOS * of);

#endif

int64_t mkfifo(const char * addr);
int64_t rmfifo(const char * addr);
int64_t write_fifo(const char * addr,const uint8_t * buf, uint64_t count );
int64_t read_fifo(const char * addr, uint8_t * buf, uint64_t count );
#define BUFF_SIZE 100

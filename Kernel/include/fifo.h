int64_t mkfifo(const char * addr);
int64_t rmfifo(const char * addr);
int64_t write_fifo(uint64_t fd ,const uint8_t * buf, uint64_t count );
int64_t read_fifo(uint64_t fd, uint8_t * buf, uint64_t count);
int64_t open_fifo(const char * addr,uint64_t mode);
#define BUFF_SIZE 100
#define WRITE 1
#define READ 2

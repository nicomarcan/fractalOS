#define READ 2
#define WRITE 1
int64_t mkfifo(const char * addr);
int64_t rmfifo(const char * addr);
int64_t write_fifo(const char * addr,const uint8_t * buf, uint64_t count );
int64_t read_fifo(const char * addr, uint8_t * buf, uint64_t count );
int64_t open_fifo(const char * addr,  uint64_t mode );
int64_t close_fifo(const char * addr,  uint64_t mode );

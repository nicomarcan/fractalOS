#include <c_string.h>
#include <c_syscall.h>
#include <fifo.h>


int64_t mkfifo(const char * addr){
	return s_mkfifo(addr);
}

int64_t rmfifo(const char * addr){
	return s_rmfifo(addr);
}

int64_t write_fifo(const char * addr,const uint8_t * buf, uint64_t count ){
	return s_write_fifo(addr,buf,count);
}

int64_t open_fifo(const char * addr, uint64_t mode ){
	return s_open_fifo(addr,mode);
}

int64_t close_fifo(const char * addr, uint64_t mode ){
	return s_close_fifo(addr,mode);
}

int64_t read_fifo(const char * addr, uint8_t * buf, uint64_t count ){
	return s_read_fifo(addr,buf,count);
}

#ifndef C_SYSCALL_H
#define C_SYSCALL_H
#include <stdint.h>
#define STDIN  0
#define STDOUT 1
#define STDERR 2

typedef struct{
	uint8_t sec,min,hour,day,month;
	uint32_t year;
} TIME;

typedef struct ProcessInfo{
	uint64_t process_count;
	uint64_t * PIDs;
	uint8_t ** descrs;
	uint8_t * status;
}ProcessInfo;

typedef struct Args{
	uint64_t argc;
	uint8_t ** argv;
	uint8_t fg;
}Args;

typedef struct mutex{
	uint64_t m;
} mutex ;

int64_t write(uint64_t fd,const uint8_t * buf, uint64_t count );
int64_t read(uint64_t fd, const uint8_t * buf, uint8_t count);
int64_t s_mkfifo(const char * addr);
int64_t s_rmfifo(const char * addr);
int64_t s_write_fifo(const char * addr,const uint8_t * buf, uint64_t count );
int64_t s_read_fifo(const char * addr, uint8_t * buf, uint64_t count );
ProcessInfo * ps();
uint64_t fkexec(void * ptr,uint8_t * descr,Args * args);
void exit();
TIME * time();
void sleep(uint64_t ticks);
void clear();
void set_color(uint8_t r, uint8_t g, uint8_t b);
void set_back_color(uint8_t r, uint8_t g, uint8_t b);
void pixel(uint32_t x,uint32_t y,uint32_t ccoord);
void kill(uint64_t pid,uint64_t mode);
void _wait();
uint64_t getPid();
uint64_t getPpid();
void mutex_init(mutex * m);
void mutex_destroy(mutex * m);
void mutex_lock(mutex * m);
void mutex_unlock(mutex * m);
void release_lock_and_sleep(mutex * m);
void _fg(uint64_t pid);
#endif

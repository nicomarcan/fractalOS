#include <c_syscall.h>

extern uint64_t _int80h(uint64_t m,uint64_t rdi,uint64_t rsi,uint64_t rdx);



int64_t read(uint64_t fd, const uint8_t * buf, uint8_t count){
  return _int80h(0,fd,buf,count);
}
int64_t write(uint64_t fd,const uint8_t * buf, uint64_t count ){
  return _int80h(1,fd,buf,count);
}

ProcessInfo * ps(){
  return _int80h(2,0,0,0);
}
uint64_t fkexec(void * ptr,uint8_t * descr,Args * args){
  return _int80h(3,ptr,descr,args);
}
void _wait(){
  _int80h(4,0,0,0);
}
void exit(){
  _int80h(5,0,0,0);
}
void kill(uint64_t pid,uint64_t mode){
  _int80h(6,pid,mode,0);
}
uint64_t getPid(){
  return _int80h(7,0,0,0);
}
uint64_t getPpid(){
  return _int80h(8,0,0,0);
}
void _fg(uint64_t pid){
  _int80h(9,pid,0,0);
}

void * mem_realloc(void * p,uint64_t m){
  return _int80h(10,p,m,0);
}
void free_mem(void * p){
  return _int80h(11,p,0,0);
}
void * memory(uint64_t m){
  return _int80h(12,m,0,0);
}

void sleep(uint64_t ticks){
  _int80h(13,ticks,0,0);
}
TIME * time(){
  return _int80h(14,0,0,0);
}


void pixel(uint32_t x,uint32_t y,uint32_t ccoord){
  _int80h(15,x,y,ccoord);
}
void clear(){
  _int80h(16,0,0,0);
}
void set_color(uint8_t r, uint8_t g, uint8_t b){
  _int80h(17,r,g,b);
}
void set_back_color(uint8_t r, uint8_t g, uint8_t b){
  _int80h(18,r,g,b);
}


uint64_t mutex_init(mutex * m){
  return _int80h(19,m,0,0);
}
void mutex_destroy(mutex * m){
  _int80h(20,m,0,0);
}
void mutex_lock(mutex * m){
  _int80h(21,m,0,0);
}
void mutex_unlock(mutex * m){
  _int80h(22,m,0,0);
}
uint64_t mutex_nameinit(mutex * m,uint8_t * name){
  return _int80h(23,m,name,0);
}
mutex * mutex_getbyname(uint8_t * name){
  return _int80h(24,name,0,0);
}
void release_lock_and_sleep(mutex * m){
  _int80h(25,m,0,0);
}


void s_ipcs(OPENED_FIFOS * of){
  _int80h(26,of,0,0);
}
int64_t s_mkfifo(const char * addr){
  return _int80h(27,addr,0,0);
}
int64_t s_rmfifo(const char * addr){
  return _int80h(28,addr,0,0);
}
int64_t s_write_fifo(uint64_t fd,const uint8_t * buf, uint64_t count ){
  return _int80h(29,fd,buf,count);
}
int64_t s_read_fifo(uint64_t fd, uint8_t * buf, uint64_t count ){
  return _int80h(30,fd,buf,count);
}
int64_t s_open_fifo(const char * addr,  uint64_t mode ){
  return _int80h(31,addr,mode,0);
}
int64_t s_close_fifo(const char * addr,  uint64_t mode ){
  return _int80h(32,addr,mode,0);
}

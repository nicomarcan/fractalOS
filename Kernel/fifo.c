#include <utils.h>
#include <fifo.h>
#include <liballoc.h>
#include <lib.h>
#include <Mutex.h>
#include <Scheduler.h>

typedef struct{
	 char * addr;
	unsigned char array[BUFF_SIZE];
	int w_pid;
	int r_pid;
	int w_fd;
	int r_fd;
	int w;
	int r;
  int  not_read;
} FIFO;

typedef struct{
	int pid;
	int next_fd;
} FD;



int fifo_count = 0;
FIFO * fifos;
FD * fds;
mutex * fifos_m;
int fd_count = 0;

static void insert(FIFO * f,unsigned char c);
static int8_t getChar(FIFO * f);


static void insert(FIFO * f,unsigned char c){
	f->array[f->w]=c;
	f->w++;
	if(f->w==BUFF_SIZE) f->w=0;
	f->not_read++;
	if(f->not_read==BUFF_SIZE+1){
		f->not_read=0;
	}
}

int64_t mkfifo(const char * addr){
	int i = 0;
	int repeated = 0;
	for(;i < fifo_count && !repeated;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			repeated = 1;
	}
        if(repeated){
                return 0;
        }
	fifo_count++;
	fifos = la_realloc(fifos,fifo_count*sizeof(FIFO ));
	fifos_m = la_realloc(fifos_m,fifo_count*sizeof(mutex));
	mutex_init(&fifos_m[fifo_count-1]);
      	fifos[fifo_count-1].addr = la_malloc(sizeof(strlen(addr)));
        strcpy((uint8_t *)fifos[fifo_count-1].addr,(uint8_t *)addr);
        fifos[fifo_count-1].w = 0;
        fifos[fifo_count-1].r = 0;
				fifos[fifo_count-1].w_pid = 0;
        fifos[fifo_count-1].r_pid = 0;
				fifos[fifo_count-1].w_fd = 0;
        fifos[fifo_count-1].r_fd = 0;
        fifos[fifo_count-1].not_read = 0;

        return fifo_count;
}

int64_t rmfifo(const char * addr){
	int i = 0;
	int found= 0;
	for(;i < fifo_count && !found;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			found = 1;
	}
        if(!found){
                return 0;
        }
				fifos[i-1] = fifos[fifo_count-1];
        fifo_count--;
        fifos = la_realloc(fifos,fifo_count*sizeof(FIFO *));
        return fifo_count;
}

int64_t open_fifo(const char * addr,uint64_t mode){

	int i = 0;
	int found= 0;
	int j=0;
	int fd=0;
	for(;i < fifo_count && !found;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			found = 1;
	}
				if(!found){
								return 0;
				}
				if(mode == WRITE){
						mutex_lock(&(fifos_m[i-1]));
					if(fifos[i-1].w_pid != 0)
						return 0;
					fifos[i-1].w_pid=currPid();
					found = 0;
					for(;j< fd_count && !found;j++){
						if(fds[j].pid == currPid()){
							fifos[i-1].w_fd= fds[j].next_fd;
							fds[j].next_fd++;
							found = 1;
						}
					}
					if(!found){
						fd_count++;
						fds = la_realloc(fds,fd_count*sizeof(FD ));
						fds[fd_count-1].pid= currPid();
						fds[fd_count-1].next_fd = 4;
						fifos[i-1].w_fd= 3;
					}
					mutex_unlock(&(fifos_m[i-1]));
					return fifos[i-1].w_fd;

				}else if(mode == READ){
					mutex_lock(&(fifos_m[i-1]));
				if(fifos[i-1].r_pid != 0)
					return 0;
				fifos[i-1].r_pid=currPid();
				found = 0;
				for(;j< fd_count && !found;j++){
					if(fds[j].pid == currPid()){
						fifos[i-1].r_fd= fds[j].next_fd;
						fds[j].next_fd++;
						found = 1;
					}
				}
				if(!found){
					fd_count++;
					fds = la_realloc(fds,fd_count*sizeof(FD ));
					fds[fd_count-1].pid= currPid();
					fds[fd_count-1].next_fd = 4;
					fifos[i-1].r_fd= 3;
				}
				mutex_unlock(&(fifos_m[i-1]));
				return fifos[i-1].r_fd;

					}else
						return 0;


}

int64_t close_fifo(const char * addr,uint64_t mode){

	int i = 0;
	int found= 0;
	int j=0;
	int fd=0;
	for(;i < fifo_count && !found;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			found = 1;
	}
				if(!found){
								return 0;
				}
				if(mode == WRITE){
						mutex_lock(&(fifos_m[i-1]));
					if(fifos[i-1].w_pid == 0)
						return 0;
					fifos[i-1].w_pid=0;
					fifos[i-1].w_fd = 0;
					mutex_unlock(&(fifos_m[i-1]));
					return 1;

				}else 	if(mode == READ){
							mutex_lock(&(fifos_m[i-1]));
						if(fifos[i-1].r_pid == 0)
							return 0;
						fifos[i-1].r_pid=0;
						fifos[i-1].r_fd = 0;
						mutex_unlock(&(fifos_m[i-1]));
						return 1;


					}else
						return 0;


}

int64_t write_fifo(uint64_t fd ,const uint8_t * buf, uint64_t count ){
	int i = 0;
	int found = 0;
	int j;
	for(;i < fifo_count && !found;i++){
		if(fifos[i].w_fd == fd && fifos[i].w_pid == currPid())
			found= 1;
	}
        if(!found ){
          return -1;
        }
	mutex_lock(&(fifos_m[i-1]));
        for(j = 0; j < count ; j++){
                insert(&fifos[i-1],buf[j]);
        }
	mutex_unlock(&(fifos_m[i-1]));
        return count;
}



int64_t read_fifo(uint64_t fd, uint8_t * buf, uint64_t count ){
	int i = 0;
	int found = 0;
	int j;
	for(;i < fifo_count && !found;i++){
		if(fifos[i].r_fd == fd && fifos[i].r_pid == currPid())
			found= 1;
	}
        if(!found ){
                return -1;
        }
	while( fifos[i-1].not_read < count){
		yield();
	}
	mutex_lock(&(fifos_m[i-1]));
        for(j = 0; j < count ; j++){
                buf[j] = getChar(&fifos[i-1]);
        }
	mutex_unlock(&(fifos_m[i-1]));
        return count;
}


static int8_t getChar(FIFO * f){
	unsigned ans = f->array[f->r];
	f->r++;
	if(f->r==BUFF_SIZE) f->r=0;
	f->not_read--;
	return ans;
}

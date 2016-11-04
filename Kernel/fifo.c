#include <utils.h>
#include <fifo.h>
#include <liballoc.h>

typedef struct{
	 char * addr;
	unsigned char array[BUFF_SIZE];
	int w;
	int r;
        int  not_read;
} FIFO;

int fifo_count = 0;
FIFO * fifos;

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
        FIFO * f = la_malloc(sizeof(FIFO));
      	f->addr = la_malloc(sizeof(strlen(addr)));
        strcpy((uint8_t *)f->addr,(uint8_t *)addr);
        f->w = 0;
        f->r = 0;
        f->not_read = 0;
        fifo_count++;
        fifos = la_realloc(fifos,fifo_count*sizeof(FIFO *));
        fifos[fifo_count - 1] = *f;
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

int64_t write_fifo(const char * addr,const uint8_t * buf, uint64_t count ){
	int i = 0;
	int found = 0;
	int j;
	for(;i < fifo_count && !found;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			found= 1;
	}
        if(!found ){
                return -1;
        }
        for(j = 0; j < count ; j++){
                insert(&fifos[i-1],buf[j]);
        }
        return count;
}



int64_t read_fifo(const char * addr, uint8_t * buf, uint64_t count ){
	int i = 0;
	int found = 0;
	int j;
	for(;i < fifo_count && !found;i++){
		if(strcmp((uint8_t *)fifos[i].addr,(uint8_t *)addr) == 0)
			found= 1;
	}
        if(!found || fifos[i-1].not_read < count){
                return -1;
        }
        for(j = 0; j < count ; j++){
                buf[j] = getChar(&fifos[i-1]);
        }
        return count;
}


static int8_t getChar(FIFO * f){
	unsigned ans = f->array[f->r];
	f->r++;
	if(f->r==BUFF_SIZE) f->r=0;
	f->not_read--;
	return ans;
}

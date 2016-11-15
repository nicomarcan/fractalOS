#include <asmlib.h>
#include <stdint.h>
#include <Mutex.h>
#define NULL 0
#define PSIZE  4096
#define PNUM   4096*8
#define PSTART 0xA00000

typedef int (*lockfunc)(void);

extern void * memset(void * p, int32_t c, uint64_t length);

struct page_table {
	char marks[PNUM];
};

struct page_table * table = (struct page_table *)NULL;
static int lock_int();
static int unlock_int();
static int lock_mutex();
static int unlock_mutex();
static int zero();
void togglelock();
static uint8_t lockset = 0;
static lockfunc lock = zero;
static lockfunc unlock = zero;


void liballoc_pagealloc_init(){
	int npages = sizeof(struct page_table)/PSIZE ;
	npages += (sizeof(struct page_table)%PSIZE == 0 ? 0 : 1);
	struct page_table * t = (struct page_table *)PSTART;
	memset(t,0,sizeof(struct page_table));
	for( int i=0 ; i<npages ; i++) {
		t->marks[i]=1;
	}
	table = t;
	return;
}

int liballoc_lock(){
	return lock();
}

int liballoc_unlock(){
	return unlock();
}

void togglelock(){
	if(lockset){
		lock = zero;
		unlock = zero;
		lockset = 0;
	} else {
		lock = lock_mutex;
		unlock = unlock_mutex;
		lockset = 1;
	}

}

void* liballoc_alloc(int npages){
	uint64_t start = -1;
	for( uint64_t i=0 ; i < PNUM-npages+1 ; i++){
		if ( table->marks[i] == 0 ){
			int found = 1;
			for( uint64_t j=i ; j<i+npages ; j++){
				if( table->marks[j] == 1 ){
					/*
					 * A marked page has been found.
					 */
					i = j+1;
					found = 0;
					break;
				}
			}
			if(found){
				/*
				 * Contiguous npages have been found,
				 * therefore they must be marked and
				 * returned.
				 */
				for( uint64_t j=i; j<i+npages ; j++){
					table->marks[j]=1;
				}
				start = i;
				break;
			}
		}
	}
	if(start != -1){
		/*
		 * the i-th page is the beginning of the
		 * required contiguous space.
		 */
		 return (void *)(PSTART + PSIZE*start);
	}
	return NULL;
}

int liballoc_free(void * p,int npages){
	if ( (uint64_t)p < (uint64_t)PSTART){
		return -1;
	}
	uint64_t start = (uint64_t)p;
	start-=PSTART;
	start/=PSIZE;
	for( uint64_t i=start ; i<start+npages ; i++){
		table->marks[i] = 0;
	}
	return 0;
}

static mutex m = {0};

static int lock_mutex(){
	mutex_lock(&m);
}

static int unlock_mutex(){
	mutex_unlock(&m);
}


static int zero(){
	return 0;
}

/* LEGACY */
int cleari = 1;
static int lock_int(){
	uint16_t fl = _readfl();
	if(!(fl&0x200)){
		cleari = 0;
	} else {
		_cli();
	}
	return 0;
}

static int unlock_int(){
	if(!cleari){
		cleari = 1;
	} else {
		_sti();
	}
	return 0;
}
/* END LEGACY */

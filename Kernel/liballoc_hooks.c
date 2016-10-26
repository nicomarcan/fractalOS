#include <asmlib.h>

static const int PSIZE = 4096;
#include <stdint.h>
static const int PNUM  = 4096;
static const int PTABLE_BASE = 0xB00000;
static const int PSTART = 0xC00000;

struct page {
	void * start;
	struct page * next ;
};

struct page * top;
struct page * used_top = NULL;


void liballoc_pagealloc_init(){
	top = (struct page *)PTABLE_BASE;
	int i;
	struct page * current;
	struct page * next;
	for(i = 0 , current = top ; i<PNUM ; i++ , current = next ){
		current -> start = PSTART + i*PSIZE;
		next = (struct page *)(PTABLE_BASE + sizeof(page)*(i+1));
		if(i != PNUM-1){
			current -> next = next;
		} else {
			current -> next = NULL;
		}
	}
}

int liballoc_lock(){
	_cli();
	return 0;
}

int liballoc_unlock(){
	_sti();
	return 0;
}

/*
 * This should be modified to
 * recursively find all possible
 * solutions
 */
void* liballoc_alloc(int npages){
	struct page * first_taken = top;
	top = top -> next;
	first_taken -> next = NULL;
	struct page * last_taken = first_taken;
	void * ret;
	void * nextp;
	int i;
	for (i = 0; i<(npages - 1) ; i++){
		nextp = last_taken -> start + PSIZE;
		int found = 0;
		for(struct page * curr = top; top != NULL ; curr = curr -> next){
			if( curr -> start == nextp) {
				
				
				
				found = 1;
				break;
			}
			
			
			
		}
		if(!found){
			/*
			 * A contiguous page has not been
			 * found for the current settings.
			 */
		}
	}
	ret = first_taken ->start;
	return ret;
	
}

int liballoc_free(void * p,int npages){
	
	
	
} 



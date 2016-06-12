#include <stdint.h>
#include <font_bitmaps.h>


static void draw_char(uint8_t *where, uint8_t character) ;


uint8_t * fb;
static int i=0;


/*
 * This function MUST be
 * called before using any
 * other functions of the
 * driver
 */
void initialize_driver(){
	/*
	* Linear framebuffer as found in
	* Pure64 docs
	*/
	fb= (uint8_t *)(*(uint32_t *)0x5080);
}

/*
 * Test function:
 * it prints all over
 * the screen
 */
void naive_print(){
	for(i=0;i<1024*768*3;i++){
		fb[i]=(uint8_t)i;
	}
	i=0;
}

void clear_screen(){
	for(i=0;i<1024*768*3;i++){
		fb[i]=0;
	}
	i=0;
}

void print_char(uint8_t character){
	draw_char((uint8_t*)fb+i,character);
	i+=8*3;
}

void print_string(char * str){
	while(*str!=0){
		print_char(*str);
		str++;
	}
}
/*
 * Adaptation of example found at osdev.org
 * by saques
 */
static void draw_char(uint8_t *where, uint8_t character) {
    int i;
    int row;
    uint8_t * tmp=where;
    uint8_t row_data;
    const uint8_t *font_data_for_char = &__font_bitmap__[(character - 0x20)*0x10];
    for (row = 0; row < 16; row++) {
        row_data = font_data_for_char[row];
		for(i=0;i<8;i++){
			if((row_data>>(8-i-1))&0x01){
				tmp[0]=0xFF;tmp[1]=0xFF;tmp[2]=0xFF;
			} else {
				tmp[0]=0x00;tmp[1]=0x00;tmp[2]=0x00;
			}
			tmp+=3;
		}
        where += 1024*3;
        tmp=where;
    }
}

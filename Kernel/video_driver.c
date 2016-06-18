#include <stdint.h>
#include <font_bitmaps.h>
#include <video_driver.h>
#define WIDTH 1024
#define HEIGHT 768
#define bPP 3
#define C_WIDTH 8
#define C_HEIGHT 16
#define DEFAULT_COLOR_R 0x00
#define DEFAULT_COLOR_G 0x00
#define DEFAULT_COLOR_B 0x00
#define DEFAULT_B_COLOR_R 0xFF
#define DEFAULT_B_COLOR_G 0xFF
#define DEFAULT_B_COLOR_B 0xFF

static void draw_char(uint8_t *where, uint8_t character) ;


uint8_t * fb;
static uint8_t char_color_r = DEFAULT_COLOR_R;
static uint8_t char_color_g = DEFAULT_COLOR_G;
static uint8_t char_color_b = DEFAULT_COLOR_B;

static uint8_t back_color_r = DEFAULT_B_COLOR_R;
static uint8_t back_color_g = DEFAULT_B_COLOR_G;
static uint8_t back_color_b = DEFAULT_B_COLOR_B;
static int i=0;
static int j=0;

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
	clear_screen();
}

/*
 * Test function:
 * it prints all over
 * the screen
 */
void naive_print(){
	for(i=0;i<WIDTH*HEIGHT*bPP;i++){
		fb[i]=(uint8_t)i;
	}
	i=0;
}

void clear_screen(){
	uint8_t * tmp = (uint8_t *)fb;
	uint64_t limit =(WIDTH*HEIGHT*bPP) +1;
	for(i=0;i<limit;i++){
		*(tmp + i )= back_color_r;
		*(tmp + i + 1)= back_color_g;
		*(tmp + i + 2)= back_color_b;
	}
	i=0;
	j=0;
}

void print_char(uint8_t character){
	draw_char((uint8_t*)fb+(i*WIDTH*C_HEIGHT+j*C_WIDTH)*bPP,
			  character);
	j++;
	if(j>=(WIDTH/C_WIDTH)){
		j=0;
		i++;
		if(i>=(HEIGHT/C_HEIGHT)){
			clear_screen();
		}
	}
}
void print(const uint8_t * sptr, uint8_t length) {
		while (length--) {
			switch (*sptr) {
				case '\t':
					for (int i=0; i<4; i++){
						print_char(' ');
					}
				case '\b':
					print_backspace();
					break;
				case '\n':
					print_newline();
					break;
				default:
					print_char(*sptr);
					break;
				}
			sptr++;
		}
}

void print_string(char * str){
	while(*str!=0){
		print_char(*str);
		str++;
	}
}

void print_newline(){
	int bk=j,count=WIDTH/C_WIDTH;
	while(bk<count){
		print_char(' ');
		bk++;
	}
}

void print_backspace() {
	if (j != 0) {
		j--;
	}
	else {
		if (i != 0) {
			i--;
			j = WIDTH / C_WIDTH - 1;
		}
	}
}

/*
 * p row
 * q column
 * ccoord: 0x00BBGGRR
 */
void print_pixel(uint32_t x,uint32_t y,uint32_t ccoord){
	uint8_t * pixel = fb + (x*WIDTH+y)*bPP;
	pixel[0]=(uint8_t)(ccoord>>16);
	pixel[1]=(uint8_t)(ccoord>>8);
	pixel[2]=(uint8_t)(ccoord);
}

/*
 * Adaptation of example found at osdev.org
 * by saques
 */
static void draw_char(uint8_t *where, uint8_t character) {
    int q;
    int row;
    uint8_t * tmp=where;
    uint8_t row_data;
    const uint8_t *font_data_for_char = &__font_bitmap__[(character - 0x20)*0x10];
    for (row = 0; row < C_HEIGHT; row++) {
        row_data = font_data_for_char[row];
		for(q=0;q<C_WIDTH;q++){
			if((row_data>>(8-q-1))&0x01){
				tmp[0]=char_color_b;tmp[1]=char_color_g;tmp[2]=char_color_r;
			} else {
				tmp[0]=back_color_b;tmp[1]=back_color_g;tmp[2]=back_color_r;
			}
			tmp+=3;
		}
        where += WIDTH*bPP;
        tmp=where;
    }
}
void set_font_color(uint8_t r, uint8_t g, uint8_t b) {
	char_color_r = r;
	char_color_g = g;
	char_color_b = b;
}
void set_back_color(uint8_t r, uint8_t g, uint8_t b) {
	back_color_r = r;
	back_color_g = g;
	back_color_b = b;
	clear_screen();
}

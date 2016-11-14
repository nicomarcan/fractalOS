#include <libgph.h>
#include <Point.h>
#include <c_syscall.h>
#include <math.h>

void printSquare(Square * s,uint32_t colour){
	int i,j;
	Point p = s->p;
	printSquare2(p.x,p.y,s->width,s->height,colour);
}

void printSquare2(uint64_t x00,uint64_t y00,uint64_t width,uint64_t height,uint32_t colour){
	int i,j;
	for(i=x00-width/2;i<x00+width/2;i++){
		for(j=y00-height/2;j<y00+height/2;j++){
			pixel(i,j,colour);
		}
	}
}

/* Midpoint Circle Algorithm - Wikipedia */
void printCircle(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour){
    int64_t x = radius;
    int64_t y = 0;
    int64_t err = 0;

    while (x >= y){
        pixel(x0 + x, y0 + y,colour);
        pixel(x0 + y, y0 + x,colour);
        pixel(x0 - y, y0 + x,colour);
        pixel(x0 - x, y0 + y,colour);
        pixel(x0 - x, y0 - y,colour);
        pixel(x0 - y, y0 - x,colour);
        pixel(x0 + y, y0 - x,colour);
        pixel(x0 + x, y0 - y,colour);

        y += 1;
        err += 1 + 2*y;
        if (2*(err-x) + 1 > 0)
        {
            x -= 1;
            err += 1 - 2*x;
        }
    }
}

void printCircleFilled(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour){
    int64_t x = radius;
    int64_t y = 0;
    int64_t err = 0;

    while (x >= y){
		printLine(x0 + x, y0 + y,x0 - x, y0 + y,colour);
		printLine(x0 + y, y0 + x,x0 - y, y0 + x,colour);
		printLine(x0 - x, y0 - y,x0 + x, y0 - y,colour);
		printLine(x0 - y, y0 - x,x0 + y, y0 - x,colour);
        y += 1;
        err += 1 + 2*y;
        if (2*(err-x) + 1 > 0)
        {
            x -= 1;
            err += 1 - 2*x;
        }
    }
}

void printCircleFilled2(uint64_t x0, uint64_t y0, uint64_t radius,uint32_t colour){
	int r2 = radius * radius;
	int area = r2 << 2;
	int rr = radius << 1;

	for (int i = 0; i < area; i++)
	{
		int tx = (i % rr) - radius;
		int ty = (i / rr) - radius;

		if (tx * tx + ty * ty <= r2)
			pixel(x0 + tx, y0 + ty, colour);
	}
}

/* http://www.brackeen.com/vga/source/djgpp20/lines.c.html */
void printLine(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint32_t colour){
  int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

  dx=x2-x1;      /* the horizontal distance of the line */
  dy=y2-y1;      /* the vertical distance of the line */
  dxabs=abs(dx);
  dyabs=abs(dy);
  sdx=signum(dx);
  sdy=signum(dy);
  x=dyabs>>1;
  y=dxabs>>1;
  px=x1;
  py=y1;

  if (dxabs>=dyabs){ /* the line is more horizontal than vertical */
    for(i=0;i<dxabs;i++){
      y+=dyabs;
      if (y>=dxabs){
        y-=dxabs;
        py+=sdy;
      }
      px+=sdx;
      pixel(px,py,colour);
    }
  } else { /* the line is more vertical than horizontal */
    for(i=0;i<dyabs;i++) {
      x+=dxabs;
      if (x>=dyabs) {
        x-=dyabs;
        px+=sdx;
      }
      py+=sdy;
      pixel(px,py,colour);
    }
  }
}

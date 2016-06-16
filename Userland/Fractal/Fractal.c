#include <stdint.h>
#include <Square.h>
#include <Point.h>
#include <Stack.h>

static Stack * squares ;

void initializeFractal(Point centre,uint32_t height,uint32_t width){
 squares = newStack();
 push(squares,newSquare(centre,height,width));
}

void step(){
  

}

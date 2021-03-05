#include <vic20.h>

# define outb(addr,val)        (*(addr) = (val))
# define inb(addr)             (*(addr))
# define SCREEN 0x1000
# define COLOR_START 0x9400  //expanded
# define COLOR_END 0x97FF

int main (void)
{
  char* screen;
  char* color;
  unsigned char x,y;

  color = (char*) COLOR_START;
  screen = (char*) SCREEN;

  
  for ( y = 0; y < 254; y++ ) {
    color[y] = 2;
  }

  for ( y = 1; y = 65; y++) {
    for( x = 0; x < 254; x++ ) {
      screen[x] = y;
    }
  }

  return 0;
}

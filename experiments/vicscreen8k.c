#include <vic20.h>
#include <peekpoke.h>

# define SCREEN 0x1000      // expanded
# define COLOR_START 0x9400  // expanded
# define COLOR_END 0x97FF
# define SCREEN_WIDTH 22
# define SCREEN_HEIGHT 23
# define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT
# define VID_RAM_LAYOUT_REG 0x9005

int main (void)
{
  unsigned char* screen;
  unsigned char* color;
  unsigned int y;
  char i,j;
  // Switch to capitals + graphics character set
  POKE(VID_RAM_LAYOUT_REG, 0xc0);

  color = (char*) COLOR_START;
  screen = (char*) SCREEN;

  
  for ( y = 0; y < SCREEN_SIZE; y++ ) {
    color[y] = 2;
  }

  for ( i = 0; i < 16; i++) {
    for ( j = 0; j < 16; j++) {
      screen[i + j * 22 ] = i + j * 16;
    }
  }

  return 0;
}

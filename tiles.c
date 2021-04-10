#include <vic20.h>

# define outb(addr,val)        (*(addr) = (val))
# define inb(addr)             (*(addr))
# define SCREEN 0x1000      // expanded
# define COLOR_START 0x9400  // expanded
# define COLOR_END 0x97FF
# define SCREEN_WIDTH 22
# define SCREEN_HEIGHT 23
# define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT

void draw_tile( char x, char y, char tile );

char tiles[8][4] = {
  "0000",
  "1111",
  "2222",
  "3333",
  "4444",
  "5555",
  "6666",
  "7777"
};

char tile_colors[8] = {
  1,0,2,3,4,5,6,7
};

int main (void)
{
  
  char i,j;
  for( i = 0; i < 16; i = i + 2 ) {
    for( j = 0; j < 16; j = j + 2) {
      draw_tile( i, j, i / 2 );
    }
  }

  return 0;
}

void draw_tile( char x, char y, char tile ) {

  unsigned int offset;
  unsigned char* screen;
  unsigned char* color;

  offset = x + y * SCREEN_WIDTH;

  color = (char*) COLOR_START;
  screen = (char*) SCREEN;

  color[offset] = tile_colors[tile];
  screen[offset] = tiles[tile][0];
  color[offset + 1] = tile_colors[tile];
  screen[offset + 1] = tiles[tile][1];
  color[offset + SCREEN_WIDTH] = tile_colors[tile];
  screen[offset + SCREEN_WIDTH] = tiles[tile][2];
  color[offset + SCREEN_WIDTH + 1] = tile_colors[tile];
  screen[offset + SCREEN_WIDTH + 1] = tiles[tile][3];

  return;
}

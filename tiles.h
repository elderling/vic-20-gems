# define SCREEN 0x1000      // expanded
# define COLOR_START 0x9400  // expanded
# define COLOR_END 0x97FF
# define SCREEN_AND_BORDER_COLOR 0x900F
# define SCREEN_WIDTH 22
# define SCREEN_HEIGHT 23
# define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT
# define VID_RAM_LAYOUT_REG 0x9005

void draw_tile( char x, char y, char tile );

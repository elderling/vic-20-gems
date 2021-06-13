#include <vic20.h>
#include <conio.h>

#define CURSOR_UP 0x91
#define CURSOR_DOWN 0x11
#define CURSOR_LEFT 0x9d
#define CURSOR_RIGHT 0x1d
#define ENTER 0x0d
#define SPACE 0x20

struct coordinate {
  signed char x;
  signed char y;
};

void update_cursor( unsigned char keystroke ); 

struct coordinate the_cursor;

unsigned char screensize_x, screensize_y;

int main() {

  unsigned char keystroke;

  screensize( &screensize_x, &screensize_y );
  clrscr();
  gotoxy( 0, 0 );
  cursor(1);
  the_cursor.x = 0;
  the_cursor.y = 0;

  while (1) {
    keystroke = cgetc();
    update_cursor( keystroke );
  }

  return 0;
}

void update_cursor(unsigned char keystroke) {

  if ( keystroke == CURSOR_UP && the_cursor.y != 0 ) {
    the_cursor.y--;
  }
  else if ( keystroke == CURSOR_DOWN && the_cursor.y != screensize_y - 1) {
    the_cursor.y++;
  }
  else if ( keystroke == CURSOR_LEFT && the_cursor.x != 0 ) {
    the_cursor.x--;
  }
  else if ( keystroke == CURSOR_RIGHT && the_cursor.x != screensize_x - 1 ) {
    the_cursor.x++;
  }

  gotoxy(0, screensize_y - 1);
  cprintf("x=%d,y=%d", the_cursor.x, the_cursor.y);
  gotoxy( the_cursor.x, the_cursor.y );

  return;
} 

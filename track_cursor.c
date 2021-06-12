#include <vic20.h>
#include <conio.h>

#define CURSOR_UP 0x91
#define CURSOR_DOWN 0x11
#define CURSOR_LEFT 0x9d
#define CURSOR_RIGHT 0x1d
#define ENTER 0x0d
#define SPACE 0x20

struct coordinate {
  unsigned char x;
  unsigned char y;
};

struct coordinate read_cursor_delta(); 

struct coordinate the_cursor;

int main() {
  struct coordinate the_delta;

  clrscr();
  gotoxy( 0, 0 );
  cursor(1);
  the_cursor.x = 0;
  the_cursor.y = 0;

  while (1) {
    the_delta = read_cursor_delta();
    the_cursor.x += the_delta.x;
    the_cursor.y += the_delta.y;
    gotoxy( the_cursor.x, the_cursor.y );
  }

  return 0;
}

struct coordinate read_cursor_delta() {
  unsigned char keystroke;
  struct coordinate the_delta;

  the_delta.x = 0;
  the_delta.y = 0;

  keystroke = cgetc();

  if ( keystroke == CURSOR_UP ) {
    the_delta.y = -1;
  }
  else if ( keystroke == CURSOR_DOWN ) {
    the_delta.y = 1;
  }
  else if ( keystroke == CURSOR_LEFT ) {
    the_delta.x = -1;
  }
  else if ( keystroke == CURSOR_RIGHT ) {
    the_delta.x = 1;
  }

  return the_delta;
} 

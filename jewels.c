#include <vic20.h>
#include <conio.h>
//#include <stdlib.h>
#include <joystick.h>

#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8

int say_hello( int );
void init_playfield();
void draw_playfield( unsigned char [8][8] );
void clear_groups_of_3_or_more();
unsigned char clear_jewel( char, char, unsigned char);

unsigned char playfield[PLAYFIELD_X][PLAYFIELD_Y] ={ 
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
{'a','b','c','d','e', 'f', 'g', 'h'},
};

unsigned char clear_jewel_tracker[PLAYFIELD_X][PLAYFIELD_Y];

int main(void) {
  unsigned char screen_x;
  unsigned char screen_y;

  screensize( &screen_x, &screen_y);

  clrscr();
  //init_playfield();
  draw_playfield( playfield );
  cprintf("%d, %d", screen_x, screen_y);
  return 0;
}

void init_playfield() {
  unsigned char x,y;
  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      playfield[x][y] = 'A';
    }
  }
}

void draw_playfield( unsigned char playfield[8][8] ) {

  unsigned char x,y;
    for (x = 0; x < PLAYFIELD_X; x++) {
      for (y = 0; y < PLAYFIELD_Y; y++) {
        cputcxy(x,y, playfield[x][y]);
      }
  }

  gotoxy(0,19);
  say_hello(1);
  
}

void clear_groups_of_3_or_more() {
  signed char x,y;
  unsigned char current_jewel_color;
    for (x = 0; x < PLAYFIELD_X; x++) {
      for (y = 0; y < PLAYFIELD_Y; y++) {
        current_jewel_color = playfield[x][y];
        if ( clear_jewel(x,y,current_jewel_color) ) {
          clear_jewel_tracker[x][y] = 1;
        }
      }
  }
}

unsigned char clear_jewel( char x, char y, unsigned char color) {
  return 1;
}

int say_hello( int num ) {
  int i;
  //clrscr();
  for( i = 0; i < num; i++ ) {
    cputs("Hello VIC-20!!!\r\n");
  }

  return 0;
}

#include <vic20.h>
#include <conio.h>
//#include <stdlib.h>
#include <joystick.h>
#include <peekpoke.h>

#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8
#define X_OFFSET 10
#define RASTER_REGISTER_LO 0x9004
#define NUMBER_OF_JEWELS 7
#define START_CHAR 1
#define EMPTY_SLOT 0
#define PETSCII_FOR_ZERO_CHAR 48

struct coordinate {
  signed char x;
  signed char y;
};

void init_playfield();
void draw_playfield_offset(  char playfield[PLAYFIELD_X][PLAYFIELD_Y], struct coordinate *offset );
void init_playfield( char [PLAYFIELD_X][PLAYFIELD_Y] );
char find_jewel_matches();
char jewel_matches( signed char, signed char, char);
char up_down_match( signed char x, signed char y, char color);
char left_right_match( signed char x, signed char y, char color);
char double_up_match( signed char x, signed char y, char color);
char double_down_match( signed char x, signed char y, char color);
char double_left_match( signed char x, signed char y, char color);
char double_right_match( signed char x, signed char y, char color);
void randomize_playfield();
void remove_jewels();
void swap_jewels(signed char x1, signed char y1, signed char x2, signed char y2);
char is_valid_move_swap( signed char x_source, signed char y_source, signed char x_destination, signed char y_destination);

char playfield[PLAYFIELD_X][PLAYFIELD_Y];
char jewel_match_grid[PLAYFIELD_X][PLAYFIELD_Y];

char raster_rand;

int main(void) {

  struct coordinate offset;
  char x1,y1,x2,y2;
  clrscr();
  cursor(1);
  bgcolor(14);
  gotoxy(0,19);
  //init_playfield(jewel_match_grid);
  init_playfield(playfield);
  randomize_playfield();
  find_jewel_matches();
  offset.x = 0;
  offset.y = 0;
  draw_playfield_offset( playfield, &offset );
  remove_jewels();
  offset.x = 10;
  offset.y = 0;
  draw_playfield_offset( jewel_match_grid, &offset );

  do {
  gotoxy(0,19);
  
  x1 = cgetc() - PETSCII_FOR_ZERO_CHAR;
  y1 = cgetc() - PETSCII_FOR_ZERO_CHAR;
  x2 = cgetc() - PETSCII_FOR_ZERO_CHAR;
  y2 = cgetc() - PETSCII_FOR_ZERO_CHAR;

  cprintf("x1=%d,y1=%d,x2=%d,y2=%d\r\n", x1, y1, x2, y2);
  offset.x = 0;
  offset.y = 0;
  draw_playfield_offset( playfield, &offset );
  swap_jewels(x1,y1,x2,y2);
  offset.x = 0;
  offset.y = 0;
  draw_playfield_offset( playfield, &offset );
  find_jewel_matches();
  remove_jewels();
  randomize_playfield();
  offset.x = 0;
  offset.y = 0;
  draw_playfield_offset( playfield, &offset );
  gotoxy(0,19);
  } while( x1 < 8 );

  return 0;
}

void update_raster_rand() {
  raster_rand = ((raster_rand >> 7) + raster_rand << 1) ^ PEEK(RASTER_REGISTER_LO);
}

void randomize_playfield() {
  signed char x,y;
  char potential_jewel;
  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      if (playfield[x][y] != EMPTY_SLOT ) {
        continue;
      }
      update_raster_rand();
      potential_jewel = START_CHAR + raster_rand % NUMBER_OF_JEWELS;
      while ( jewel_matches(x,y,potential_jewel) ) {
        if ( potential_jewel < (START_CHAR + NUMBER_OF_JEWELS - 2) ) {
          potential_jewel++;
        }
        else {
          potential_jewel = START_CHAR;
        }
      } 
      playfield[x][y] = potential_jewel;
    }
  }
}

void init_playfield( char playfield[PLAYFIELD_X][PLAYFIELD_Y] ) {
  char x,y;
  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      playfield[x][y] = EMPTY_SLOT;
    }
  }
}

void draw_playfield_offset( char playfield[PLAYFIELD_X][PLAYFIELD_Y], struct coordinate *offset ) {

  char x,y;
    for (x = 0; x < PLAYFIELD_X; x++) {
      for (y = 0; y < PLAYFIELD_Y; y++) {
        textcolor(playfield[x][y]);
        cputcxy(x + offset->x, y + offset->y, playfield[x][y] + PETSCII_FOR_ZERO_CHAR);
      }
  }
}

// Doesn't check that any of the parameters are outside of array boundaries
char is_valid_move_swap( signed char x_source, signed char y_source, signed char x_destination, signed char y_destination) {

  if ( x_source == x_destination ) {
     if ( y_destination == y_source + 1 || y_destination == y_source - 1) {
      return 1;
     }
  }

  if ( y_source == y_destination ) {
     if ( x_destination == x_source + 1 || x_destination == x_source - 1) {
      return 1;
     }
  }

  return 0;
}

void swap_jewels(signed char x1, signed char y1, signed char x2, signed char y2) {
  char temp;

  temp = playfield[x1][y1];
  playfield[x1][y1] = playfield[x2][y2];
  playfield[x2][y2] = temp;

  return;
}

void remove_jewels() {
  signed char stack_top, x, y;

  for ( x = 0; x < PLAYFIELD_X; x++ ) {
    stack_top = PLAYFIELD_Y - 1;
    for ( y = PLAYFIELD_Y - 1; y >= 0; y-- ) {
      if ( jewel_match_grid[x][y] == 1 ) {
        continue;
      }
      playfield[x][stack_top] = playfield[x][y];
      stack_top--;
    }

    for ( y = stack_top; y >= 0; y-- ) {
      playfield[x][y] = EMPTY_SLOT;
    }
  }
}

char find_jewel_matches() {
  signed char x,y;

  char found_matches = 0;

  char current_jewel_color;

  init_playfield(jewel_match_grid);

  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      current_jewel_color = playfield[x][y];
      if ( current_jewel_color == EMPTY_SLOT ) { 
        continue; 
      }
      if ( jewel_matches(x,y,current_jewel_color) ) {
        found_matches = 1;
        jewel_match_grid[x][y] = 1;
      }
    }
  }

  return found_matches;
}

char jewel_matches( signed char x, signed char y, char color) {
  if ( up_down_match( x, y, color) ) {
    return 1;
  }
  if ( left_right_match( x, y, color) ) {
    return 1;
  }
  if (up_down_match( x, y, color) ) {
    return 1;
  }
  if (double_left_match( x, y, color) ) {
    return 1;
  }
  if (double_right_match( x, y, color) ) {
    return 1;
  }
  if (double_up_match( x, y, color) ) {
    return 1;
  }
  if (double_down_match( x, y, color) ) {
    return 1;
  }
  return 0;
}

char up_down_match( signed char x, signed char y, char color) {
  if ( y == 0 || y == PLAYFIELD_Y - 1 ) {
    return 0;
  }
  if ( playfield[x][y-1] == color && playfield[x][y+1] == color ) {
    return 1;
  }
  return 0;
}

char left_right_match( signed char x, signed char y, char color) {
  if ( x == 0 || x == PLAYFIELD_X - 1 ) {
    return 0;
  }
  if ( playfield[x-1][y] == color && playfield[x+1][y] == color ) {
    return 1;
  }
  return 0;
}

char double_up_match( signed char x, signed char y, char color) {
  if ( y < 2 ) {
    return 0;
  }
  if ( playfield[x][y-1] == color && playfield[x][y-2] == color ) {
    return 1;
  }

  return 0;
}

char double_down_match( signed char x, signed char y, char color) {
  if ( y > PLAYFIELD_Y - 3 ) {
    return 0;
  }

  if ( playfield[x][y+1] == color && playfield[x][y+2] == color ) {
    return 1;
  }

  return 0;
}

char double_left_match( signed char x, signed char y, char color) {
  if ( x < 2 ) {
    return 0;
  }
  if ( playfield[x-1][y] == color && playfield[x-2][y] == color ) {
    return 1;
  }

  return 0;
}

char double_right_match( signed char x, signed char y, char color) {
  if ( x > PLAYFIELD_X - 3 ) {
    return 0;
  }

  if ( playfield[x+1][y] == color && playfield[x+2][y] == color ) {
    return 1;
  }

  return 0;
}

#include <vic20.h>
#include <conio.h>
#include <peekpoke.h>

#define EMPTY_SLOT 0
#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8
#define CURSOR_UP 0x91
#define CURSOR_DOWN 0x11
#define CURSOR_LEFT 0x9d
#define CURSOR_RIGHT 0x1d
#define NUMBER_OF_GEMS 7
#define PETSCII_FOR_ZERO_CHAR 48
#define RASTER_REGISTER_LO 0x9004
#define START_CHAR 1

struct coordinate {
  char x;
  char y;
};

void initialize_display(void);
void initialize_game_state(void);
void initialize_playfield(void);
void render_display(void);
char get_command(void);
void do_command(char command);
void draw_playfield(void);
char up_down_match( struct coordinate *gem_location, char gem );
char left_right_match( struct coordinate *gem_location, char gem );
char double_down_match( struct coordinate *gem_location, char gem );
char double_up_match( struct coordinate *gem_location, char gem );
char double_left_match( struct coordinate *gem_location, char gem );
char double_right_match( struct coordinate *gem_location, char gem );
char gem_matches( struct coordinate *gem_location, char gem);
void update_raster_rand(void);
void randomize_playfield(void);

char raster_rand;

char playfield[PLAYFIELD_X][PLAYFIELD_Y];

struct coordinate game_cursor;

int main() {

  initialize_display();
  initialize_game_state();
  initialize_playfield();
  randomize_playfield();
  render_display();

  while (1) {
    do_command(get_command());
    render_display();
  }

  return 0;
}

void initialize_display() {

  clrscr();
  gotoxy( 0, 0 );
  bgcolor( COLOR_BLACK );
  bordercolor( COLOR_BLACK );
  cursor(1);

  return;
}

void initialize_game_state() {
  game_cursor.x = 0;
  game_cursor.y = 0;

  return;
}

void render_display() {
  draw_playfield();
  gotoxy(0, PLAYFIELD_Y);
  textcolor( COLOR_WHITE );
  cprintf("x=%2d,y=%2d", game_cursor.x, game_cursor.y);
  gotoxy( game_cursor.x, game_cursor.y );
}

char get_command() {
    return cgetc();
}

void do_command(char command) {

  if ( command == CURSOR_UP && game_cursor.y != 0 ) {
    game_cursor.y--;
  }
  else if ( command == CURSOR_DOWN && game_cursor.y != PLAYFIELD_Y - 1) {
    game_cursor.y++;
  }
  else if ( command == CURSOR_LEFT && game_cursor.x != 0 ) {
    game_cursor.x--;
  }
  else if ( command == CURSOR_RIGHT && game_cursor.x != PLAYFIELD_X - 1 ) {
    game_cursor.x++;
  }

  return;
}

void initialize_playfield() {
  char x,y;
  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      playfield[x][y] = EMPTY_SLOT;
    }
  }
}

void draw_playfield() {

  char x,y;
    for (x = 0; x < PLAYFIELD_X; x++) {
      for (y = 0; y < PLAYFIELD_Y; y++) {
        textcolor(playfield[x][y]);
        cputcxy(x, y, playfield[x][y] + PETSCII_FOR_ZERO_CHAR);
      }
  }
}

char gem_matches( struct coordinate *gem_location, char gem) {
  if ( up_down_match( gem_location, gem) ) {
    return 1;
  }
  if ( left_right_match( gem_location, gem) ) {
    return 1;
  }
  if (up_down_match( gem_location, gem) ) {
    return 1;
  }
  if (double_left_match( gem_location, gem) ) {
    return 1;
  }
  if (double_right_match( gem_location, gem) ) {
    return 1;
  }
  if (double_up_match( gem_location, gem) ) {
    return 1;
  }
  if (double_down_match( gem_location, gem) ) {
    return 1;
  }
  return 0;
}

char up_down_match( struct coordinate *gem_location, char gem ) {
  char x = gem_location->x;
  char y = gem_location->y;
  if ( y == 0 || y == PLAYFIELD_Y - 1 ) {
    return 0;
  }
  if ( playfield[x][y-1] == gem && playfield[x][y+1] == gem ) {
    return 1;
  }

  return 0;
}

char left_right_match( struct coordinate *gem_location, char gem) {
  char x = gem_location->x;
  char y = gem_location->y;

  if ( x == 0 || y == PLAYFIELD_X - 1 ) {
    return 0;
  }
  if ( playfield[x-1][y] == gem && playfield[x+1][y] == gem ) {
    return 1;
  }

  return 0;
}

char double_up_match( struct coordinate *gem_location, char gem) {
  char x = gem_location->x;
  char y = gem_location->y;

  if ( y > PLAYFIELD_Y - 3  ) {
    return 0;
  }
  if ( playfield[x][y+1] == gem && playfield[x][y+2] == gem ) {
    return 1;
  }

  return 0;
}

char double_down_match( struct coordinate *gem_location, char gem ) {
  char x = gem_location->x;
  char y = gem_location->y;

  if ( y < 2 ) {
    return 0;
  }

  if ( playfield[x][y-1] == gem && playfield[x][y-2] == gem ) {
    return 1;
  }

  return 0;
}

char double_left_match( struct coordinate *gem_location, char gem) {
  char x = gem_location->x;
  char y = gem_location->y;

  if ( x < 2 ) {
    return 0;
  }
  if ( playfield[x-1][y] == gem && playfield[x-2][y] == gem ) {
    return 1;
  }

  return 0;
}

char double_right_match( struct coordinate *gem_location, char gem) {
  char x = gem_location->x;
  char y = gem_location->y;

  if ( x > PLAYFIELD_X - 3 ) {
    return 0;
  }

  if ( playfield[x+1][y] == gem && playfield[x+2][y] == gem ) {
    return 1;
  }

  return 0;
}

void update_raster_rand() {
  raster_rand = ((raster_rand >> 7) + raster_rand << 1) ^ PEEK(RASTER_REGISTER_LO);
}

void randomize_playfield() {
  char x,y;
  char potential_gem;
  struct coordinate gem_location;
  for (x = 0; x < PLAYFIELD_X; x++) {
    for (y = 0; y < PLAYFIELD_Y; y++) {
      if (playfield[x][y] != EMPTY_SLOT ) {
        continue;
      }
      update_raster_rand();
      potential_gem = START_CHAR + raster_rand % NUMBER_OF_GEMS;
      gem_location.x = x;
      gem_location.y = y;
      while ( gem_matches(&gem_location, potential_gem) ) {
          potential_gem++;
          if ( potential_gem > ( START_CHAR + NUMBER_OF_GEMS - 1 )) {
            potential_gem = START_CHAR;
          }
      } 
      playfield[x][y] = potential_gem;
    }
  }
}

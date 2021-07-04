#include <vic20.h>
#include <conio.h>

#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8
#define CURSOR_UP 0x91
#define CURSOR_DOWN 0x11
#define CURSOR_LEFT 0x9d
#define CURSOR_RIGHT 0x1d

void initialize_display();
void initialize_game_state();
void render_display();
char get_command();
void do_command(char command);

struct coordinate {
  char x;
  char y;
};

struct coordinate game_cursor;

int main() {

  initialize_display();
  initialize_game_state();
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
  cursor(1);

  return;
}

void initialize_game_state() {
  game_cursor.x = 0;
  game_cursor.y = 0;

  return;
}

void render_display() {
  gotoxy(0, PLAYFIELD_Y);
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

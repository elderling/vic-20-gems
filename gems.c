#include <vic20.h>
#include <conio.h>

#define PLAYFIELD_X 8
#define PLAYFIELD_Y 8

void initialize_display();
void initialize_game_state();
void render_display();

struct coordinate {
  char x;
  char y;
};

struct coordinate game_cursor;

int main() {

  initialize_display();
  initialize_game_state();
  render_display();

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

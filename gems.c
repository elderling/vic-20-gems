#include <vic20.h>
#include <conio.h>

void initialize_display();

int main() {

  initialize_display();

  return 0;
}

void initialize_display() {

  clrscr();
  gotoxy( 0, 0 );
  cursor(1);

  return;
}

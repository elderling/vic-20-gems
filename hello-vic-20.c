#include <vic20.h>
#include <conio.h>

int say_hello( int num );

int main(void) {
  say_hello( 11 );
  return 0;
}

int say_hello( int num ) {
  int i;
  clrscr();
  for( i = 0; i < num; i++ ) {
    cputs("Hello VIC-20!!!\r\n");
  }

  return 0;
}

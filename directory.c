#include <stdio.h>
#include <cbm.h>

int main() {
  char i,n,lfn, open_result, chkin_result, data_char, foo, data;

  const char* name = "$";
  lfn = 1;

  open_result = cbm_open( lfn, 8, 0, name );

  chkin_result = cbm_k_chkin( lfn );

  for ( i = 0; i < 254; i++ ) {
    foo = cbm_read( 1, &data, 1);
    printf("%c,", data);
  }

  cbm_k_close( lfn );

  return 0;
}

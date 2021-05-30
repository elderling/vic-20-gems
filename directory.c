#include <stdio.h>
#include <cbm.h>

int main() {
  unsigned char i,lfn, open_result, chkin_result, foo, data;

  const unsigned char* name = "advent-20-01,s,r";
  printf("'%s'", name);
  lfn = 1;

  open_result = cbm_open( lfn, 8, 0, name );

  if ( open_result != 0 ) {
    printf("Error: %d", open_result);
    cbm_k_close( lfn );
    return -1;
  }

  chkin_result = cbm_k_chkin( lfn );

  printf("chkin_result = '%d'", chkin_result);

  for ( i = 0; i < 254; i++ ) {
    foo = cbm_read( lfn, &data, 1);
    printf("%x,", data);
  }

  cbm_k_close( lfn );

  printf("\n");
  printf("'%s'", name);

  return 0;
}

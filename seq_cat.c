#include <stdio.h>
#include <cbm.h>

int main() {
  unsigned char lfn, open_result, chkin_result, bytes_read, data;

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

  do {
    bytes_read = cbm_read( lfn, &data, 1);
    if ( data == 0x0d ) {
        printf("\n");
    }
    else {
      printf("%c", data);
    }
  }
  while ( bytes_read > 0 );

  cbm_k_close( lfn );

  printf("\n");
  printf("'%s'", name);

  return 0;
}

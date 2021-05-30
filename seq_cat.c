#include <stdlib.h>
#include <stdio.h>
#include <cbm.h>

int main() {
  unsigned char string_position, lfn, open_result, chkin_result, bytes_read, data;
  unsigned char number_string[10];
  unsigned char** endptr;
  long int the_actual_number;
  long int the_long_total = 0;

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

  number_string[0] = 0;
  string_position = 0;
  do {
    
    bytes_read = cbm_read( lfn, &data, 1);

    if ( data == 0x0d ) {
        //printf("\n");
        number_string[string_position] = 0;
        // printf("%s\n", number_string);
        the_actual_number = strtol(number_string, endptr, 0);

        the_long_total += the_actual_number;
        printf("%ld\n", the_actual_number);

        string_position = 0;
    }
    else {
      //printf("%c", data);
      number_string[string_position++] = data;
    }

  }
  while ( bytes_read > 0 );

  cbm_k_close( lfn );

  printf("\n");
  printf("'%s'", name);
  printf("The Long Total = %ld", the_long_total);

  return 0;
}

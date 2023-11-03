#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "reading_structs.h"
#include "metacommand.h"
#include "record.h"

void print_prompt() {
  printf("db > ");
}

int main(int argc, char** argv) {
  if(argc < 2) {
    printf("Must supply database filename.\n");
    exit(EXIT_FAILURE);
  }

  // TODO(#4): [Data-Structure] Change this.
  char* filename = argv[1];
  Table* table = db_open(filename);

  int loop = 1;
  InputBuffer* input_buffer = get_input_buffer();

  while(loop) {

    print_prompt();
    read_buffer(input_buffer);

    int is_meta = do_meta_command(input_buffer, table);
    StatementResult statement_result = prepare_statement(input_buffer, table);

    if(statement_result == NOT_STATEMENT && !is_meta) {
      printf("Invalid command.\n");
    }

  }

  return 0;
}

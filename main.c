#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "./src/reading_structs.h"
#include "./src/metacommand.h"
#include "./src/record.h"

void print_prompt() {
  printf("db > ");
}

int main() {

  // TODO: [Data-Structure] Change this.
  Table* table = get_table();

  int loop = 1;
  InputBuffer* input_buffer = get_input_buffer();
  Statement* statement = get_statement();

  while(loop) {

    print_prompt();
    read_buffer(input_buffer);

    do_meta_command(input_buffer);
    StatementResult statement_result = prepare_statement(input_buffer, statement, table);
  }

  return 0;
}

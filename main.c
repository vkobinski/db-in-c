#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include "./src/reading_structs.h"
#include "./src/metacommand.h"

#define STRING_MAX_SIZE 50

void print_prompt() {
  printf("db > ");
}
int main() {

  int loop = 1;
  InputBuffer* input_buffer = get_input_buffer();
  Statement* statement = get_statement();

  while(loop) {

    print_prompt();
    read_buffer(input_buffer);

    do_meta_command(input_buffer);
    StatementResult statement_result = prepare_statement(input_buffer, statement);

    if(statement_result == STATEMENT_SUCCESS) {
      if(statement->type == SELECT) {
        printf("Select Statement.\n");
      }
      if(statement->type == INSERT) {
        printf("Insert Statement.\n");
      }

      continue;
    }

    printf("Unrecognized command %s.\n", input_buffer->buffer);

  }

  return 0;
}

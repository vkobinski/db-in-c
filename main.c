#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "./reading_structs.h"

#define STRING_MAX_SIZE 50

typedef enum {
  META_EXIT,
  META_INFO
} MetaCommand;

typedef enum {

} MetaResult;


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

    if(strcmp(input_buffer->buffer, ".exit") == 0) {
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
    }

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

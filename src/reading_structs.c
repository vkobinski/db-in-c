#include "./reading_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

void lower_case_string(char* string) {
  for( ; *string; ++string) *string = tolower(*string);
}

InputBuffer* get_input_buffer() {
  InputBuffer* input_buffer = (InputBuffer*) malloc(sizeof(InputBuffer));
  input_buffer->buffer = NULL;
  input_buffer->length = 0;
  input_buffer->input_length = 0;

  return input_buffer;
}

void close_input_buffer(InputBuffer* input_buffer) {
  free(input_buffer->buffer);
  free(input_buffer);
}

void read_buffer(InputBuffer* input_buffer) {
  ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->length), stdin);

  if(bytes_read <= 0)  {
    printf("Error reading input, because of %d\n", errno);
    exit(EXIT_FAILURE);
  }

  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read-1] = 0;

}



Statement* get_statement() {
  Statement* statement = (Statement*) malloc(sizeof(Statement));
  statement->count = 0;
  statement->tokens = malloc(STRING_MAX_SIZE*50);

  return statement;
}

StatementResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
  char* token;
  token = strtok(input_buffer->buffer, " ");
  lower_case_string(token);

  if(strcmp(token, "select") == 0)  {
    statement->type = SELECT;
  }
  if(strcmp(token, "insert") == 0) {
    statement->type = INSERT;
  }

  token = strtok(NULL, " ");

  while(token != NULL) {
    size_t size = strlen(token);

    if(size >= STRING_MAX_SIZE-1) {
      return STATEMENT_STRING_TOO_LONG;
    }

    statement->tokens[statement->count] = malloc(sizeof(char)*STRING_MAX_SIZE);
    statement->tokens[statement->count] = token;

    statement->count++;
    token = strtok(NULL, " ");
  }

  return STATEMENT_SUCCESS;
}



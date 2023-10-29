#include "./reading_structs.h"
#include "./record.h"
#include <sys/types.h>
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

int find_highest_id(Table* table) {
  return table->num_rows;
}

SelectResult execute_select(Table* table) {

  for(uint32_t i = 0; i < table->num_rows; i++) {
    Row* row = deserialize_row(table, i+1);
    printf("(%"PRIu32", %s, %s)\n", row->id, row->username, row->email);
  }

  return SELECT_SUCCESS;
}

InsertResult execute_insert(Table* table, Row* row) {

  if(table->num_rows == TOTAL_ROWS) return INSERT_MAX_ROWS_ERROR;

  row->id = find_highest_id(table) + 1;
  serialize_row(table, row);

  table->num_rows++;

  return INSERT_SUCCESS;
}

StatementResult prepare_insert(InputBuffer* input_buffer, Statement* statement, Table* table) {
  char* token;
  token = strtok(NULL, " ");

  if(strncmp(token, "(", 1) != 0) return STATEMENT_INSERT_WRONG_ARGUMENTS;

  token++;
  token[strlen(token)-1] = '\0';

  char* username = strtok(token, ",");
  char* email = strtok(NULL, ",");

  Row* row = get_row();
  memcpy(row->username, username, strlen(username));
  memcpy(row->email, email, strlen(email));

  if(execute_insert(table, row) == INSERT_SUCCESS) {
    printf("Executed.\n");
  }

  return STATEMENT_SUCCESS;
}

StatementResult prepare_statement(InputBuffer* input_buffer, Statement* statement, Table* table) {
  char* token;
  token = strtok(input_buffer->buffer, " ");
  lower_case_string(token);

  if(strcmp(token, "select") == 0) {
    execute_select(table);
    return STATEMENT_SUCCESS;
  } else if(strcmp(token, "insert") == 0) {
    return prepare_insert(input_buffer, statement, table);
  }
  else return NOT_STATEMENT;

}

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

int find_higher_id(Records* records) {
  return records->row_count + 1;
}

SelectResult execute_select(Records* records) {
  for(int i = 0; i < records->row_count; i++) {
    Row* row = records->rows[i];
    printf("(%"PRIu32", %s, %s)\n", row->id, row->username, row->email);
  }

  return SELECT_SUCCESS;
}

InsertResult execute_insert(Records* records, Row* row) {

  if(records->row_count == MAX_ROWS) return INSERT_MAX_ROWS_ERROR;

  row->id = find_higher_id(records);
  records->rows[records->row_count] = row;
  records->row_count += 1;

  return INSERT_SUCCESS;
}

StatementResult prepare_insert(InputBuffer* input_buffer, Statement* statement, Records* records) {
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

  if(execute_insert(records, row) == INSERT_SUCCESS) {
    printf("Executed.\n");
  }

  return STATEMENT_SUCCESS;
}

StatementResult prepare_statement(InputBuffer* input_buffer, Statement* statement, Records* records) {
  char* token;
  token = strtok(input_buffer->buffer, " ");
  lower_case_string(token);

  if(strcmp(token, "select") == 0) {
    execute_select(records);
    return STATEMENT_SUCCESS;
  } else if(strcmp(token, "insert") == 0) {
    return prepare_insert(input_buffer, statement, records);
  }
  else return NOT_STATEMENT;

}



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

uid_t find_highest_id(Table* table) {
  return table->num_rows;
}

SelectResult execute_select(Table* table) {

  for(uint32_t i = 0; i < table->num_rows; i++) {
    printf("(");
    print_row(table, i+1);
    printf("\n");
  }

  return SELECT_SUCCESS;
}

InsertResult execute_insert(Table* table, Row* row) {

  //if(table->num_rows == total_rows(table->row_info)) return INSERT_MAX_ROWS_ERROR;

  //row->id = find_highest_id(table) + 1;
  //serialize_row(table, row);


  //return INSERT_SUCCESS;

  return INSERT_SUCCESS;
}

StatementResult prepare_insert(Table* table) {
  char* token;
  token = strtok(NULL, "(");

  RowInformation* info = table->row_info;

  token[strlen(token)-1] = '\0';

  Column** columns = malloc(sizeof(Column) * MAX_COLUMNS);
  uintmax_t num_i;
  double_t num_d;
  char* err;

  Column* col_id = malloc(sizeof(Column));
  col_id->id = find_highest_id(table) + 1;
  col_id->type = ID;
  col_id->col_pos = 0;

  columns[0] = col_id;

  char* cur_tok = strtok(token, ",");

  for(ssize_t i = 1; i < info->col_count; i++) {
    Column* col = malloc(sizeof(Column));
    columns[i] = col;

    char* cur_token = calloc(strlen(cur_tok), sizeof(char));
    memcpy(cur_token, cur_tok, strlen(cur_tok));
    char* trimmed = trim(cur_token);

    switch (info->col_types[i]) {
      case ID:
        assert(0 && "Id should not be in insert statement");
        break;
      case INT:
        num_i = strtoumax(trimmed, NULL, 10);
        if (num_i == UINTMAX_MAX && errno == ERANGE) {
          printf("Could not convert %s into a Integer for column %s",
              trimmed,
              info->col_names[i]);
          exit(EXIT_FAILURE);
        }
        col->col_pos = i;
        col->integer = num_i;
        col->type = INT;
        free(trimmed);
        break;
      case REAL:
        num_d = strtod(trimmed, &err);
        if (*err != 0) {
            printf("Could not convert '%s' into a Real for column %s\n",
                   trimmed,
                   info->col_names[i]);
            exit(EXIT_FAILURE);
        } else {
            col->col_pos = i;
            col->real = num_d;
            col->type = REAL;
        }
        free(trimmed);
      break;
      case TEXT:
        col->col_pos = i;
        col->text = trimmed;
        col->type = TEXT;
        break;
      default:
        assert(0 && "Column has type not known");
        exit(EXIT_FAILURE);

    }
  cur_tok = strtok(NULL, ",");
  }

  uid_t id = find_highest_id(table) + 1;
  serialize_row(table, id, columns);
  printf("Executed.\n");

  table->num_rows++;

  return STATEMENT_SUCCESS;
}

StatementResult prepare_statement(InputBuffer* input_buffer, Table* table) {
  char* token;
  token = strtok(input_buffer->buffer, " ");
  lower_case_string(token);

  if(strcmp(token, "select") == 0) {
    execute_select(table);
    return STATEMENT_SUCCESS;
  } else if(strcmp(token, "insert") == 0) {
    return prepare_insert(table);
  }
  else return NOT_STATEMENT;

}

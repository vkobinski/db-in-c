#ifndef READING_STRUCTS_H
#define READING_STRUCTS_H

#include <sys/types.h>
#include "./record.h"

#define STRING_MAX_SIZE 50

void lower_case_string(char* string);

typedef struct {
  char* buffer;
  size_t length;
  ssize_t input_length;
} InputBuffer;

InputBuffer* get_input_buffer();

void close_input_buffer(InputBuffer* input_buffer);

void read_buffer(InputBuffer* input_buffer);

typedef enum  {
  SELECT,
  INSERT,
} Statement_Type;

typedef struct {
  Statement_Type type;
  char** tokens;
  int count;
} Statement;

typedef enum {
  STATEMENT_STRING_TOO_LONG,
  STATEMENT_INSERT_WRONG_ARGUMENTS,
  STATEMENT_SELECT_WRONG_ARGUMENTS,
  STATEMENT_SUCCESS,
  NOT_STATEMENT,
} StatementResult;

typedef enum {
  SELECT_SUCCESS,
  SELECT_NOT_FOUND,
  SELECT_ERROR,
} SelectResult;

typedef enum {
  INSERT_SUCCESS,
  INSERT_MAX_ROWS_ERROR,
} InsertResult;

int find_higher_id(Table* table);

Statement* get_statement();

StatementResult prepare_select(InputBuffer* input_buffer, Statement* statement, Table* table);
StatementResult prepare_insert(InputBuffer* input_buffer, Statement* statement, Table* table);
StatementResult prepare_statement(InputBuffer* input_buffer, Statement* statement, Table* table);

#endif

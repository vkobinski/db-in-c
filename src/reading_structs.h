#ifndef READING_STRUCTS_H
#define READING_STRUCTS_H

#include <sys/types.h>

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
  STATEMENT_SUCCESS,
} StatementResult;

Statement* get_statement();

StatementResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

#endif

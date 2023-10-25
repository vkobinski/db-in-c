#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>

#define STRING_MAX_SIZE 50
#define MAX_ROWS 50

typedef struct {
  uint32_t id;
  char username[STRING_MAX_SIZE];
  char email[STRING_MAX_SIZE];
} Row;

// TODO: [Data-Structure] implement a better memory implementation, then a b-tree implementation
typedef struct {
  Row* rows[MAX_ROWS];
  int row_count;
} Records;

Row* get_row();

#endif

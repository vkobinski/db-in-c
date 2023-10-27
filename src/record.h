#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define STRING_MAX_SIZE 50
#define MAX_TABLE_PAGES 100
#define ROWS_PER_PAGE (PAGE_SIZE/sizeof(Row))
#define TOTAL_ROWS (ROWS_PER_PAGE * MAX_TABLE_PAGES)

typedef struct {
  uint32_t id;
  char username[STRING_MAX_SIZE];
  char email[STRING_MAX_SIZE];
} Row;

Row* get_row();

typedef struct {
  Row* rows[ROWS_PER_PAGE];
} Page;

// TODO: [Data-Structure] implement a better memory implementation, then a b-tree implementation
typedef struct {
  Page* pages[MAX_TABLE_PAGES];
  uint32_t num_rows;
} Table;

Table* get_table();
Row* row_slot(Table* table, uint32_t row_id);
void set_row(Table* table, Row* row);
Page* row_page(Table* table, uint32_t row_id);

#endif

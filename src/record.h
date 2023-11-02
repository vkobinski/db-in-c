#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>
#include <stdlib.h>
#define PAGE_SIZE 4096
#define STRING_MAX_SIZE 50
#define MAX_TABLE_PAGES 100
#define ROWS_PER_PAGE (PAGE_SIZE/sizeof(Row))
#define TOTAL_ROWS (ROWS_PER_PAGE * MAX_TABLE_PAGES)

// A page is just a space in memory, this will make the serialization and deserialization easier
typedef struct {
  uint32_t id;
  char username[STRING_MAX_SIZE];
  char email[STRING_MAX_SIZE];
} Row;

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void* pages[MAX_TABLE_PAGES];
} Pager;

// TODO(#7): [Data-Structure] implement a better memory implementation, then a b-tree implementation
typedef struct {
  Pager* pager;
  uint32_t num_rows;
} Table;

void* row_slot(Table* table, uint32_t row_id);
void* row_page(Table* table, uint32_t row_id);

Row* get_row();
Row* deserialize_row(Table* table, uint32_t row_id);
void serialize_row(Table* table, Row* row);

void additional_rows_flush(Table* table);
void page_flush(Pager* pager, ssize_t pos);
void save_pager_content(Table* table);

void* get_page(Table* table, uint32_t page_num);

Pager* pager_open(const char* filename);
void pager_flush(Table* table);
Table* db_open(const char* filename);
void db_close(Table* table);

#endif

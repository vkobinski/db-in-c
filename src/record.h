#ifndef RECORD_H
#define RECORD_H

#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "string_utils.h"
#define PAGE_SIZE 4096
#define TEXT_MAX_SIZE 100
#define MAX_TABLE_PAGES 100
#define MAX_COLUMNS 50

typedef struct {
  void* column_data;
} Row;

typedef enum {
  ID,
  INT,
  REAL,
  TEXT
} ColumnType;

typedef struct {
  ColumnType type;
  ssize_t col_pos;
  union {
    uint32_t id;
    char* text;
    uint32_t integer;
    double_t real;
  };
} Column;

typedef struct {
  int file_descriptor;
  uint32_t file_length;
  void* header;
  void* pages[MAX_TABLE_PAGES];
} Pager;

typedef struct {
  ssize_t row_size;
  ssize_t col_count;
  ColumnType col_types[MAX_COLUMNS];
  char** col_names;
} RowInformation;

// TODO(#7): [Data-Structure] implement a better memory implementation, then a b-tree implementation
typedef struct {
  Pager* pager;
  uint32_t num_rows;
  RowInformation* row_info;
  char table_name[50];
} Table;

void* row_slot(Table* table, int32_t row_id);
void* row_page(Table* table, uint32_t row_id);


Row* get_row(RowInformation* info);
Row* deserialize_row(Table* table, ssize_t row_pos);
void serialize_row(Table* table, ssize_t row_pos, Column** columns);

void additional_rows_flush(Table* table);
void page_flush(Pager* pager, ssize_t pos);
void save_pager_content(Table* table);

void* get_page(Table* table, uint32_t page_num);
Pager* pager_open(const char* filename);
void pager_flush(Table* table);

RowInformation* create_row_information(char* table_name, char* table_description);
size_t col_size(ColumnType type);
// If the stop_col argument is -1, then the function returns
// the size of the entire row
ssize_t row_size(RowInformation* info, ssize_t stop_col);
size_t row_col_size(ColumnType type);
size_t rows_per_page(RowInformation* info);
size_t total_rows(RowInformation* info);

ssize_t col_pos_by_name(RowInformation* info, char* name);

// Reading row values functions
Row* read_row_data(Table* table, ssize_t row_pos);
ssize_t col_offset(RowInformation* info, ssize_t col_pos);
uint32_t read_row_id(RowInformation* info, Row* row);
char* read_row_text(RowInformation* info, Row* row, ssize_t col_pos);
uint32_t read_row_int(RowInformation*info, Row* row, ssize_t col_pos);
double_t read_row_real(RowInformation* info, Row* row, ssize_t col_pos);
void print_row(Table* table, ssize_t row_pos);
// Storing row values functions
void store_row_id(Table* table, Row* row, uint32_t col_data);
void store_row_text(Table* table, Row* row, ssize_t col_pos, char* col_data);
void store_row_int(Table* table, Row* row, ssize_t col_pos, uint32_t col_data);
void store_row_real(Table* table, Row* row, ssize_t col_pos, double_t col_data);

size_t row_information_size(Table *table);
void read_row_information(Table *table);
void store_row_information(Table *table);

Table* db_open(const char* filename);
void db_close(Table* table);

#endif

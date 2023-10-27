#include "record.h"
#include <stdlib.h>
#include <string.h>

Row* get_row() {

  Row* row = (Row*) malloc(sizeof(Row));
  row->id = 0;

  return row;
}

Row* row_slot(Table* table, uint32_t row_id) {

  Page* row_page = table->pages[row_id/ROWS_PER_PAGE];
  if(row_page == NULL) {
    row_page = malloc(sizeof(Page));
    table->pages[row_id/ROWS_PER_PAGE] = row_page;
  }

  return row_page->rows[row_id % ROWS_PER_PAGE];

}

Page* row_page(Table* table, uint32_t row_id) {

  Page* row_page = table->pages[row_id/ROWS_PER_PAGE];
  if(row_page == NULL) {
    row_page = malloc(sizeof(Page));
    table->pages[row_id/ROWS_PER_PAGE] = row_page;
  }

  return row_page;

}

void set_row(Table* table, Row* row) {

  Page* page = row_page(table, row->id);

  page->rows[row->id] = row;

  table->num_rows += 1;

}

Table* get_table() {

  Table* table = (Table*) malloc(sizeof(Table));
  table->num_rows = 0;

  for(uint32_t i = 0; i < MAX_TABLE_PAGES; i++) {
    table->pages[i] = NULL;
  }

  return table;
}

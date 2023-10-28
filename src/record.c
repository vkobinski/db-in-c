#include "record.h"
#include <stdlib.h>
#include <string.h>

Row* get_row() {

  Row* row = (Row*) malloc(sizeof(Row));
  row->id = 0;

  return row;
}

Page* row_page(Table* table, uint32_t row_id) {

  Page* row_page = table->pages[row_id/ROWS_PER_PAGE];
  if(row_page == NULL) {
    row_page = malloc(sizeof(Page));
    table->pages[row_id/ROWS_PER_PAGE] = row_page;
  }

  return row_page;

}

Row* row_slot(Table* table, uint32_t row_id) {

  Page* page_for_row = row_page(table, row_id);
  return page_for_row->rows[row_id % ROWS_PER_PAGE];

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

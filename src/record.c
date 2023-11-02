#include "record.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

const ssize_t id_offset = sizeof(uint32_t);
const ssize_t username_offset = id_offset;
const ssize_t email_offset = username_offset + sizeof(char) * STRING_MAX_SIZE;
const ssize_t string_sizes = sizeof(char) * STRING_MAX_SIZE;

Row* deserialize_row(Table* table, uint32_t row_id) {
  char* row_start = row_slot(table, row_id);

  Row* row = get_row();
  row->id = row_id;

  memcpy(row->username, row_start + username_offset, string_sizes);
  memcpy(row->email, row_start + email_offset, string_sizes);

  return row;
}

void serialize_row(Table* table, Row* row) {
  char* row_start = row_slot(table, row->id);

  memcpy(row_start, &(row->id), sizeof(uint32_t));
  memcpy(row_start + username_offset, &(row->username), string_sizes);
  memcpy(row_start + email_offset, &(row->email), string_sizes);
}

Row* get_row() {

  Row* row = (Row*) malloc(sizeof(Row));
  row->id = 0;

  return row;
}

void* get_page(Table* table, uint32_t page_num) {
  Pager* pager = table->pager;

  if(pager->pages[page_num] == NULL) {
    // Cache miss.
    void* page = malloc(PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    if(pager->file_length % PAGE_SIZE) {
      num_pages += 1;
    }

    if(page_num <= num_pages) {
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);

      if(bytes_read == -1) {
        printf("Error reading file: %d\n", errno);
        exit(-1);
      }

      pager->pages[page_num] = page;
    }

  }

  return pager->pages[page_num];

}

void* row_page(Table* table, uint32_t row_id) {
  Pager* pager = table->pager;
  void* row_page = pager->pages[row_id/ROWS_PER_PAGE];

  if(row_page == NULL) {
    row_page = malloc(sizeof(Row) * ROWS_PER_PAGE);
    pager->pages[row_id/ROWS_PER_PAGE] = row_page;
  }
  return row_page;

}

void* row_slot(Table* table, uint32_t row_id) {

  uint32_t page_num = row_id/ROWS_PER_PAGE;
  char* page = get_page(table, page_num);

  ssize_t row_offset = (sizeof(Row) * ((row_id-1) % ROWS_PER_PAGE));

  return page + row_offset;

}

Pager* pager_open(const char* filename) {
  int file = open(filename, O_RDWR, O_CREAT, S_IWUSR, S_IRUSR);
  if(file == -1) {
    printf("Unable to open db file.\n");
    exit(-1);
  }

  Pager* pager = malloc(sizeof(Pager));
  pager->file_descriptor = file;
  pager->file_length = lseek(file, 0, SEEK_END);

  for(uint32_t i = 0; i < MAX_TABLE_PAGES; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
}

Table* db_open(const char* filename) {
  Table* table = (Table*) malloc(sizeof(Table));
  table->pager = pager_open(filename);

  table->num_rows = table->pager->file_length / sizeof(Row);

  return table;
}

void additional_rows_flush(Table* table) {
  uint32_t num_full_pages = (table->num_rows / ROWS_PER_PAGE);
  uint32_t additional_rows = table->num_rows % ROWS_PER_PAGE;

  Pager* pager = table->pager;
  int fd = pager->file_descriptor;

  if(additional_rows > 0) {
    uint32_t page_num = num_full_pages;

    if(pager->pages[page_num] != NULL) {
      lseek(fd, page_num * PAGE_SIZE, SEEK_SET);
      uint64_t bytes_written = write(fd, pager->pages[page_num], additional_rows * sizeof(Row));

      if(bytes_written < sizeof(Row)) {
        ssize_t not_written_rows = (bytes_written - (additional_rows * sizeof(Row))) / sizeof(Row);
        printf("Could not write %zd rows to disk.\n", not_written_rows);
        exit(EXIT_FAILURE);
      }

      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }
}

void page_flush(Pager* pager, ssize_t pos) {
  int fd = pager->file_descriptor;
  lseek(fd, pos * PAGE_SIZE, SEEK_SET);
  ssize_t bytes_written = write(fd, pager->pages[pos], PAGE_SIZE);

    if(bytes_written < PAGE_SIZE) {
      printf("Could not write page %zd to disk.\n", pos);
      exit(EXIT_FAILURE);
    }

    free(pager->pages[pos]);
    pager->pages[pos] = NULL;

}

void save_pager_content(Table* table) {
  Pager* pager = table->pager;
  uint32_t num_full_pages = (table->num_rows / ROWS_PER_PAGE);

  for(ssize_t i = 0; i < num_full_pages; i++) {
    page_flush(pager, i);
  }

  additional_rows_flush(table);

}

void db_close(Table* table) {
  save_pager_content(table);
  int result = close(table->pager->file_descriptor);

  if(result == -1) {
    printf("Error closing the db file.\n");
    exit(EXIT_FAILURE);
  }

  free(table->pager);
  free(table);
}

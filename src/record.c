#include "record.h"
#include "reading_structs.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Row *deserialize_row(Table *table, ssize_t row_pos) {

  void *row_start = row_slot(table, row_pos);
  Row *row = get_row(table->row_info);
  return row;
}

Row *read_row_data(Table *table, ssize_t row_pos) {
  void *row_start = row_slot(table, row_pos);
  Row *row = malloc(sizeof(Row));
  row->column_data = row_start;

  return row;
}

void print_row(Table *table, ssize_t row_pos) {
  RowInformation *info = table->row_info;

  Row *row = read_row_data(table, row_pos);

  uint32_t cur_id;
  uint32_t cur_int;
  char *cur_str;
  double_t cur_real;
  size_t size_col;

  for (ssize_t i = 0; i < info->col_count; i++) {

    switch (info->col_types[i]) {
    case ID:
      cur_id = read_row_id(table->row_info, row);
      if (i != info->col_count - 1)
        printf("%d, ", cur_id);
      else
        printf("%d)", cur_id);
      break;
    case INT:
      cur_int = read_row_int(table->row_info, row, i);
      if (i != info->col_count - 1)
        printf("%d, ", cur_int);
      else
        printf("%d)", cur_int);
      break;
    case REAL:
      cur_real = read_row_real(table->row_info, row, i);
      if (i != info->col_count - 1)
        printf("%f, ", cur_real);
      else
        printf("%f)", cur_real);
      break;
    case TEXT:
      cur_str = read_row_text(table->row_info, row, i);
      if (i != info->col_count - 1)
        printf("%s, ", cur_str);
      else
        printf("%s)", cur_str);
      break;
    default:
      assert(0 && "Column has type not known");
      exit(EXIT_FAILURE);
    }
  }
}

void serialize_row(Table *table, ssize_t row_id, Column **columns) {
  RowInformation *info = table->row_info;
  Row *row = read_row_data(table, row_id);

  for (ssize_t i = 0; i < info->col_count; i++) {
    Column *cur = columns[i];

    switch (cur->type) {
    case ID:
      store_row_id(table, row, cur->id);
      break;
    case INT:
      store_row_int(table, row, cur->col_pos, cur->integer);
      break;
    case REAL:
      store_row_real(table, row, cur->col_pos, cur->real);
      break;
    case TEXT:
      store_row_text(table, row, cur->col_pos, cur->text);
      break;
    default:
      assert(0 && "Column has type not known");
      exit(EXIT_FAILURE);
    }

    free(cur);
  }
  free(row);
  free(columns);
}

Row *get_row(RowInformation *info) {

  Row *row = (Row *)malloc(sizeof(Row));
  row->column_data = malloc(row_size(info, -1));

  return row;
}

void *get_page(Table *table, uint32_t page_num) {
  Pager *pager = table->pager;

  if (pager->pages[page_num] == NULL) {
    // Cache miss.
    void *page = calloc(1, PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    if (pager->file_length % PAGE_SIZE) {
      num_pages += 1;
    }

    if (page_num <= num_pages) {
      lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);

      if (bytes_read == -1) {
        printf("Error reading file: %d\n", errno);
        exit(-1);
      }

      pager->pages[page_num] = page;
    }
  }

  return pager->pages[page_num];
}

void *row_page(Table *table, uint32_t row_id) {
  Pager *pager = table->pager;
  size_t rows_page = rows_per_page(table->row_info);

  void *row_page = pager->pages[row_id / rows_page];

  if (row_page == NULL) {
    row_page = malloc(row_size(table->row_info, -1) * rows_page);
    pager->pages[row_id / rows_page] = row_page;
  }
  return row_page;
}

void *row_slot(Table *table, int32_t row_id) {

  RowInformation *info = table->row_info;
  size_t rows_page = rows_per_page(info);
  uint32_t page_num = row_id / rows_page;
  char *page = get_page(table, page_num);

  size_t size = row_size(info, -1);

  ssize_t row_offset = size * ((row_id - 1) % rows_per_page(info));

  return page + row_offset;
}

Pager *pager_open(const char *filename) {
  int file = open(filename, O_RDWR, O_CREAT, S_IWUSR, S_IRUSR);
  if (file == -1) {
    printf("Unable to open db file.\n");
    exit(-1);
  }

  Pager *pager = malloc(sizeof(Pager));
  pager->file_descriptor = file;
  pager->file_length = lseek(file, 0, SEEK_END);

  for (uint32_t i = 0; i < MAX_TABLE_PAGES; i++) {
    pager->pages[i] = NULL;
  }

  return pager;
}

Table *db_open(const char *filename) {
  Table *table = (Table *)malloc(sizeof(Table));

  table->pager = pager_open(filename);

  table->row_info = NULL;

  // TODO(#8): Load row information from file
  // const char* s = "(name:text, email:text)";
  // char* c = (char*)malloc(strlen(s)+1);
  // strcpy(c,s);
  //

  // TODO(#9): Read table name from file
  // memcpy(table->table_name, "user", 5);

  // table->row_info = create_row_information(c);

  // size_t size = row_size(table->row_info, -1);
  // table->num_rows = table->pager->file_length / size;

  return table;
}

void additional_rows_flush(Table *table) {
  size_t rows_page = rows_per_page(table->row_info);
  uint32_t num_full_pages = (table->num_rows / rows_page);
  uint32_t additional_rows = table->num_rows % rows_page;

  size_t size = row_size(table->row_info, -1);

  Pager *pager = table->pager;
  int fd = pager->file_descriptor;

  if (additional_rows > 0) {
    uint32_t page_num = num_full_pages;

    if (pager->pages[page_num] != NULL) {
      lseek(fd, page_num * PAGE_SIZE, SEEK_SET);
      uint64_t bytes_written =
          write(fd, pager->pages[page_num], additional_rows * (size));

      if (bytes_written < size) {
        ssize_t not_written_rows =
            (bytes_written - (additional_rows * size)) / size;
        printf("Could not write %zd rows to disk.\n", not_written_rows);
        exit(EXIT_FAILURE);
      }

      free(pager->pages[page_num]);
      pager->pages[page_num] = NULL;
    }
  }
}

void page_flush(Pager *pager, ssize_t pos) {
  int fd = pager->file_descriptor;
  lseek(fd, pos * PAGE_SIZE, SEEK_SET);
  ssize_t bytes_written = write(fd, pager->pages[pos], PAGE_SIZE);

  if (bytes_written < PAGE_SIZE) {
    printf("Could not write page %zd to disk.\n", pos);
    exit(EXIT_FAILURE);
  }

  free(pager->pages[pos]);
  pager->pages[pos] = NULL;
}

void save_pager_content(Table *table) {
  Pager *pager = table->pager;
  size_t rows_page = rows_per_page(table->row_info);
  uint32_t num_full_pages = (table->num_rows / rows_page);

  for (ssize_t i = 0; i < num_full_pages; i++) {
    page_flush(pager, i);
  }

  additional_rows_flush(table);
}

ssize_t col_pos_by_name(RowInformation *info, char *name) {
  ssize_t col_pos = -1;

  for (ssize_t i = 0; i < info->col_count; i++) {
    if (strcmp(name, info->col_names[i]) == 0) {
      col_pos = i;
    }
  }

  if (col_pos == -1) {
    printf("Could not find column [%s]", name);
    return -1;
  }

  return col_pos;
}

ssize_t col_offset(RowInformation *info, ssize_t col_pos) {
  ssize_t offset = row_size(info, col_pos);
  return offset;
}

uint32_t read_row_id(RowInformation *info, Row *row) {
  void *col_addres = row->column_data;
  return *((uint32_t *)col_addres);
}

char *read_row_text(RowInformation *info, Row *row, ssize_t col_pos) {
  void *col_addres = row->column_data + col_offset(info, col_pos);
  return (char *)col_addres;
}

uint32_t read_row_int(RowInformation *info, Row *row, ssize_t col_pos) {
  void *col_addres = row->column_data + col_offset(info, col_pos);
  return *((uint32_t *)col_addres);
}

double_t read_row_real(RowInformation *info, Row *row, ssize_t col_pos) {
  void *col_addres = row->column_data + col_offset(info, col_pos);
  return *((double_t *)col_addres);
}

void store_row_information(Table *table) {
  TableDescriptionSerialized *desc =
      malloc(sizeof(TableDescriptionSerialized));

  void *page = table->pager->pages[0];
  desc->table_name_len = strlen(table->table_name);
  desc->col_count = table->row_info->col_count;
}

void store_row_text(Table *table, Row *row, ssize_t col_pos, char *col_data) {
  RowInformation *info = table->row_info;
  void *col_addres = row->column_data + col_offset(info, col_pos);
  strncpy((char *)col_addres, col_data, TEXT_MAX_SIZE + 1);
}

void store_row_id(Table *table, Row *row, uint32_t col_data) {
  uint32_t *col_addres = row->column_data;
  *col_addres = col_data;
}

void store_row_int(Table *table, Row *row, ssize_t col_pos, uint32_t col_data) {
  RowInformation *info = table->row_info;
  uint32_t *col_addres =
      (uint32_t *)(row->column_data + col_offset(info, col_pos));
  *col_addres = col_data;
}

void store_row_real(Table *table, Row *row, ssize_t col_pos,
                    double_t col_data) {
  RowInformation *info = table->row_info;
  double_t *col_addres =
      (double_t *)(row->column_data + col_offset(info, col_pos));
  *col_addres = col_data;
}

size_t row_col_size(ColumnType type) {
  switch (type) {
  case ID:
    return sizeof(uint32_t);
    break;
  case INT:
    return sizeof(int32_t);
    break;
  case REAL:
    return sizeof(double_t);
    break;
  case TEXT:
    return sizeof(char) * TEXT_MAX_SIZE;
    break;
  default:
    assert(0 && "Column has type not known");
    exit(EXIT_FAILURE);
  }
}

ssize_t row_size(RowInformation *info, ssize_t stop_col) {
  ssize_t stop = info->col_count;

  if (stop_col != -1)
    stop = stop_col;

  ssize_t size = 0;
  for (ssize_t i = 0; i < stop; i++) {
    switch (info->col_types[i]) {
    case ID:
      size += row_col_size(info->col_types[i]);
      break;
    case INT:
      size += row_col_size(info->col_types[i]);
      break;
    case REAL:
      size += row_col_size(info->col_types[i]);
      break;
    case TEXT:
      size += row_col_size(TEXT);
      break;
    default:
      assert(0 && "Column has type not known");
      exit(EXIT_FAILURE);
    }
  }
  return size;
}

size_t rows_per_page(RowInformation *info) {

  size_t size = row_size(info, -1);
  return PAGE_SIZE / size;
}

size_t total_rows(RowInformation *info) {
  return rows_per_page(info) * MAX_TABLE_PAGES;
}

size_t col_size(ColumnType type) {
  size_t size;
  switch (type) {
  case ID:
    size = sizeof(uint32_t);
    break;
  case INT:
    size = sizeof(int32_t);
    break;
  case REAL:
    size = sizeof(double_t);
    break;
  case TEXT:
    size = sizeof(char) * TEXT_MAX_SIZE;
    break;
  default:
    assert(0 && "Column has type not known");
    exit(EXIT_FAILURE);
  }

  return size;
}

// The table information will be of this form: (type:column_name, type:
// column_name)
RowInformation *create_row_information(char *table_name,
                                       char *table_description) {

  table_description[strlen(table_description) - 1] = '\0';

  int *size = malloc(sizeof(int));
  char **columns = split(table_description, ',', size);

  RowInformation *row_information =
      (RowInformation *)malloc(sizeof(RowInformation));
  row_information->col_names = malloc(sizeof(char *) * (*size + 1));
  row_information->col_count = *size + 1;

  row_information->col_types[0] = ID;

  const char *s = "id";
  char *c = (char *)malloc(strlen(s) + 1);
  strcpy(c, s);

  row_information->col_names[0] = c;

  for (int i = 0; i < *size; i++) {
    char **column = split(columns[i], ':', NULL);

    char *tipo = trim(column[1]);
    char *nome = trim(column[0]);

    lower_case_string(tipo);
    lower_case_string(nome);

    if (strcmp(nome, "id") == 0) {
      printf(
          "The id column is managed by the DATABASE, remove the %d paramn.\n",
          i + 1);
      exit(EXIT_FAILURE);
    }

    ColumnType col_type;

    if (strcmp(tipo, "int") == 0)
      col_type = INT;
    else if (strcmp(tipo, "real") == 0)
      col_type = REAL;
    else if (strcmp(tipo, "text") == 0)
      col_type = TEXT;
    else {
      printf("Could not parse row column of type: %s\n", tipo);
      exit(EXIT_FAILURE);
    }

    row_information->col_types[i + 1] = col_type;
    row_information->col_names[i + 1] =
        (char *)malloc(sizeof(strlen(nome)) + 1);
    memcpy(row_information->col_names[i + 1], nome, strlen(nome) + 1);

    free(tipo);
    free(nome);
  }

  row_information->row_size = row_size(row_information, -1);

  return row_information;
}

void db_close(Table *table) {
  save_pager_content(table);
  int result = close(table->pager->file_descriptor);

  if (result == -1) {
    printf("Error closing the db file.\n");
    exit(EXIT_FAILURE);
  }

  free(table->pager);
  free(table);
}

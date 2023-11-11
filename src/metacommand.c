#include "./metacommand.h"
#include "./record.h"

void print_help() {
  printf("SQLite Commands:\n");
  printf("\tINSERT (arg1, arg2...)\tThe args types need to align to the table column order.\n");
  printf("\tSELECT \tReturns all rows informations.\n");
  printf("\tCREATE_TABLE table_name (colname1:coltype1, colname1:coltype2...) \tCreates table based on the arguments,\n\
      \tthe types must be one of the following: Real, Int, Text.\n");
}

void print_table_info(Table* table) {
  RowInformation* info = table->row_info;
  printf("Table %s:\n", table->table_name);

  for(int i = 0; i < info->col_count; i++) {
    printf("\tColumn Name: %s, ", info->col_names[i]);

    switch(info->col_types[i]) {
      case ID:
        printf("Id");
        break;
      case INT:
        printf("Integer");
        break;
      case REAL:
        printf("Real");
        break;
      case TEXT:
        printf("Text");
        break;
    }
    printf("\n");

  }
}



MetaCommand recognize_meta_command(InputBuffer* input_buffer) {

  if(strcmp(input_buffer->buffer, ".exit") == 0) return META_EXIT;
  if(strcmp(input_buffer->buffer, ".info") == 0) return META_INFO;
  if(strcmp(input_buffer->buffer, ".help") == 0) return META_HELP;

  return META_UNRECOGNIZED;
}

int do_meta_command(InputBuffer* input_buffer, Table* table) {

  if(strncmp(input_buffer->buffer,".",1) != 0) {
    return 0;
  }

  MetaCommand command = recognize_meta_command(input_buffer);

  switch (command){
    case META_EXIT:
      db_close(table);
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
      break;
    case META_INFO:
      print_table_info(table);
      break;
    case META_HELP:
      print_help();
      break;
    case META_UNRECOGNIZED:
      printf("Unrecognized command %s.\n", input_buffer->buffer);
      break;
    default:
      assert(0 && "Unreachable code.");
  }
  return 1;
}



#include "./metacommand.h"
#include "./record.h"

void print_table_info(Table* table) {
RowInformation* info = table->row_info;
  printf("Table:\n");

  for(int i = 0; i < info->columns_count; i++) {
    printf("\tColumn Name: %s, ", info->row_names[i]);

    switch(info->row_types[i]) {
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
    case META_UNRECOGNIZED:
      printf("Unrecognized command %s.\n", input_buffer->buffer);
      break;
    default:
      assert(0 && "Unreachable code.");
  }
  return 1;
}



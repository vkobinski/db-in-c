#include "./metacommand.h"

MetaCommand recognize_meta_command(InputBuffer* input_buffer) {

  if(strcmp(input_buffer->buffer, ".exit") == 0) return META_EXIT;
  if(strcmp(input_buffer->buffer, ".info") == 0) return META_INFO;

  return META_UNRECOGNIZED;
}

void do_meta_command(InputBuffer* input_buffer) {

  if(strncmp(input_buffer->buffer,".",1) != 0) {
    return;
  }

  MetaCommand command = recognize_meta_command(input_buffer);

  switch (command){
    case META_EXIT:
      // TODO: Add the saving proccess here.
      close_input_buffer(input_buffer);
      exit(EXIT_SUCCESS);
      break;
    case META_INFO:
      assert(0 && "Not implemented.");
      break;
    case META_UNRECOGNIZED:
      printf("Unrecognized command %s.\n", input_buffer->buffer);
      break;
    default:
      assert(0 && "Unreachable code.");
  }
}



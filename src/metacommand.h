#ifndef METACOMMAND_H
#define METACOMMAND_H

#include "./reading_structs.h"
#include "./record.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef enum {
  META_EXIT,
  META_INFO,
  META_HELP,
  META_UNRECOGNIZED,
} MetaCommand;

MetaCommand recognize_meta_command(InputBuffer* input_buffer);

int do_meta_command(InputBuffer* input_buffer, Table* table);
void print_table_info(Table* table);
void print_help();

#endif

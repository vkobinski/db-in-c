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
  META_UNRECOGNIZED,
} MetaCommand;

MetaCommand recognize_meta_command(InputBuffer* input_buffer);

void do_meta_command(InputBuffer* input_buffer);

#endif

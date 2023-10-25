#include "record.h"
#include <stdlib.h>

Row* get_row() {

  Row* row = (Row*) malloc(sizeof(Row));
  row->id = 0;

  return row;
}

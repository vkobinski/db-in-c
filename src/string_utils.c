#include "string_utils.h"

size_t ltrim(char* str) {
  size_t left_spaces = 0;

  for(size_t i = 0;; i++) {
    if(isspace(str[i])) left_spaces++;
    else break;
  }

  return left_spaces;
}

size_t rtrim(char* str) {

  size_t right_spaces = 0;
  size_t strsize = strlen(str);

  char* str_end = str + strsize - 1;

  while(isspace(*str_end)) {
    right_spaces++;
    str_end--;
  }

  return right_spaces;
}

char* trim(char* str) {
  size_t triml = ltrim(str);
  size_t trimr = rtrim(str);

  size_t strsize = strlen(str) - (triml + trimr);

  char* trimmed = (char*) malloc(sizeof(char) * strsize);

  size_t x = 0;

  for(size_t i = triml; i < strlen(str) - trimr; i++) {
    trimmed[x] = str[i];
    x++;
  }

  trimmed[strsize] = '\0';

  return trimmed;

}

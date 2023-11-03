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

  free(str);

  return trimmed;

}

char** split(char* str, const char delimeter) {

  StartFinishPair pairs[50];
  size_t len = strlen(str);

  size_t last_start = 0;
  int last_pair = 0;

  for(size_t i = 0; i < strlen(str); i++) {

    if(str[i] == delimeter) {
      StartFinishPair current_pair;

      current_pair.str_start = last_start;
      current_pair.str_finish = i;

      pairs[last_pair] = current_pair;

      last_pair++;
      last_start = i;
    }
  }

  char** tokens = malloc(len);

  for(size_t i = 0; i < last_pair; i++) {


  }

}

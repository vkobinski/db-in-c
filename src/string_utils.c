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

// TODOOOOO(#10): If delimiter is not found, a segmentation error occurs
char** split(char* str, const char delimiter, int* size) {
    StartFinishPair pairs[MAX_TOKEN_QTD];
    size_t len = strlen(str);

    size_t last_start = 0;
    int last_pair = 0;

    for (size_t i = 0; i < len; i++) {
        if (str[i] == delimiter) {
            StartFinishPair current_pair;
            current_pair.str_start = last_start;
            current_pair.str_finish = i;

            if(last_pair == 50) {
              printf("Token limit for split function is: %d", MAX_TOKEN_QTD);
              return NULL;
            }

            pairs[last_pair] = current_pair;
            last_pair++;
            last_start = i + 1;
        }
    }


    if(size != NULL) *size = last_pair + 1;

    char** tokens = (char**)malloc((last_pair + 1) * sizeof(char*));
    size_t total_size = 0;

    if(last_pair == 0) {
        tokens[0] = (char*)malloc(strlen(str) + 1);
        strcpy(tokens[0], str);
        tokens[0][strlen(str)+1] = '\0';

        return tokens;
    }

    for (size_t i = 0; i < last_pair; i++) {
        size_t current_size = pairs[i].str_finish - pairs[i].str_start;
        tokens[i] = (char*)malloc(current_size + 1);
        strncpy(tokens[i], str + pairs[i].str_start, current_size);
        tokens[i][current_size] = '\0';
        total_size++;
    }

    if (total_size < last_pair + 1) {
        size_t current_size = len - pairs[last_pair - 1].str_finish - 1;
        tokens[total_size] = (char*)malloc(current_size + 1);
        strncpy(tokens[total_size], str + pairs[last_pair - 1].str_finish + 1, current_size);
        tokens[total_size][current_size] = '\0';
    }

    return tokens;
}

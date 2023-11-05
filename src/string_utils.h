#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_TOKEN_QTD 50

// Trim functions
// This trim implementation returns the trimmed string and frees the original str
size_t ltrim(char* str);
size_t rtrim(char* str);
char* trim(char* str);

void lower_case_string(char* string);

char** split(char* str, const char delimeter, int* size);

typedef struct {
  size_t str_start;
  size_t str_finish;
} StartFinishPair;



#endif

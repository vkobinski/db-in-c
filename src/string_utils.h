#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

// Trim functions
size_t ltrim(char* str);
size_t rtrim(char* str);
char* trim(char* str);

#endif

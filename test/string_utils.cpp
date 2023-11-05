#include "gtest/gtest.h"
#include <malloc.h>
#include <string.h>
#include <iostream>

extern "C" {
#include "../src/string_utils.h"
}

TEST(trim, ok) {

  const char* s = "   hello   ";
  char* c = (char*)malloc(strlen(s)+1);
  strcpy(c,s);

  EXPECT_STREQ(trim(c), "hello");
}

TEST(sqlite, ok) {

  const char* s = "   hello   ";
  char* c = (char*)malloc(strlen(s)+1);
  strcpy(c,s);

  EXPECT_STRNE(trim(c), " hello   ");
}

TEST(split, ok) {
  const char* s = "hello, how are you, fine";
  char* c = (char*)malloc(strlen(s)+1);
  strcpy(c,s);

  int* size = (int*) calloc(1,sizeof(int));
  split(c, ',', size);

  EXPECT_EQ(*size, 3);

}

TEST(split2, ok) {
  const char* s = "hello, how are you, fine";
  char* c = (char*)malloc(strlen(s)+1);
  strcpy(c,s);

  int* size = (int*) calloc(1,sizeof(int));

  EXPECT_STREQ(split(c,',',size)[0], "hello");
}

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

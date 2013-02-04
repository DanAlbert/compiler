// Don't forget gtest.h, which declares the testing framework.
// Step 1: include appropriate files
#include <string.h>
#include <stdlib.h>
#include "../token.h"
#include "../token.cpp"
#include "../messages.h"
#include "gtest/gtest.h"

// Step 2: create test macro cases
// Tests factorial of negative numbers.
TEST(TokenTest, TokenTypeToString ) {
  // This test is named "isComment", and belongs to the "LexerTest"
  // test case.
  EXPECT_EQ("number", TokenTypeToString('1'));
  //EXPECT_EQ("string",TokenTypetoString("string"));
}

// Step 3. Call RUN_ALL_TESTS() in main().
//
// We do this by linking in src/gtest_main.cc file, which consists of
// a main() function which calls RUN_ALL_TESTS() for us.
//
// This runs all the tests you've defined, prints the result, and
// returns 0 if successful, or 1 otherwise.
//
// Did you notice that we didn't register the tests?  The
// RUN_ALL_TESTS() macro magically knows about all the tests we
// defined.  Convenient.

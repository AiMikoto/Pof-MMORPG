#include "lib/uuid.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_trie()
{
  TEST("TESTING TRIE ADDITION");
  trie t;
  t.add("cat");
  t.add("car");
  t.add("carpet");
  t.add("comet");
  PASS;
  TEST("TESTING TRIE ACCESS");
  if((t.has("cat")) && (t.has("comet")) && !(t.has("cart")))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING TRIE REMOVAL");
  t.remove("carp");
  t.remove("car");
  if((t.has("carpet")) && !(t.has("car")))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
}

int main()
{
  test_trie();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

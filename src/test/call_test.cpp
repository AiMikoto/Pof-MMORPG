#include "lib/call.h"
#include <cstdio>
#include <iostream>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_call_creation()
{
  TEST("TESTING CALL CREATION");
  call *c = new call();
  delete c;
  PASS;
}

void test_call_access()
{
  TEST("TESTING ACCESS OF A PARAMETER TO A CALL");
  call *c = new call();
  c -> tree().put("endpoint", "parameter");
  if(c -> tree().get<std::string>("endpoint") != "parameter")
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING ACCESS OF A NESTED PARAMETER");
  c -> tree().put("nested1.nested2.3", 5);
  if(c -> tree().get<int>("nested1.nested2.3") != 5)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING FAILURE OF ACCESS OF A NULL PARAMETER");
  try
  {
    c -> tree().get<int>("a");
    FAIL;
  }
  catch(std::exception& e)
  {
    PASS;
  }
  delete c;
}

int main()
{
  test_call_creation();
  test_call_access();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

#include "world_structure/world.h"
#include <cstdio>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_world_creation()
{
  TEST("TESTING WORLD CREATION");
  world *w = new world();
  delete w;
  PASS;
}

void test_object_addition()
{
  TEST("TESTING ADDITION OF AN OBJECT TO A WORLD");
  position p1 = {1,2,3};
  position p2 = {4,5,6};
  object o = surface(p1, p2);
  world *w = new world();
  if(w -> add_object(o) <= 0)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  delete w;
}

void test_object_retrieval()
{
  TEST("TESTING RETRIEVAL OF AN OBJECT FROM A WORLD");
  object o = entity({1,2,3});
  world *w = new world();
  w -> get_object(w -> add_object(o));
  PASS;
  delete w;
}

int main()
{
  test_world_creation();
  test_object_addition();
  test_object_retrieval();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

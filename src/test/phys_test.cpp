#include "phys/octree.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_octree()
{

}

int main()
{
  test_octree();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

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
  aabb base;
  base.minx = -1000;
  base.miny = -1000;
  base.minz = -1000;
  base.maxx =  1000;
  base.maxy =  1000;
  base.maxz =  1000;
  octree t(base);
  TEST("TESTING ADDITION OF BOXES TO OCTREE");
  aabb box1, box2, box3;
  box1.minx = -1;
  box1.miny = -1;
  box1.minz = -1;
  box1.maxx =  1;
  box1.maxy =  1;
  box1.maxz =  1;
  box2.minx =  2;
  box2.miny =  2;
  box2.minz =  2;
  box2.maxx =  3;
  box2.maxy =  3;
  box2.maxz =  3;
  t.insert(1, box1);
  t.insert(2, box2);
  PASS;
  TEST("TESTING COLLISION OF BOXES WITHIN OCTREE")
  box3.minx =  2.5;
  box3.miny =  2.5;
  box3.minz =  2.5;
  box3.maxx =  3.5;
  box3.maxy =  3.5;
  box3.maxz =  3.5;
  std::set<int> collisions = t.get_collisions(box3);
  if((collisions.size() == 1) && (collisions.find(2) != collisions.end()))
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
  test_octree();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

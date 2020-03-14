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
  aabb box;
  box.minx = -1;
  box.miny = -1;
  box.minz = -1;
  box.maxx =  1;
  box.maxy =  1;
  box.maxz =  1;
  t.insert(1, box);
  box.minx =  2;
  box.miny =  2;
  box.minz =  2;
  box.maxx =  3;
  box.maxy =  3;
  box.maxz =  3;
  t.insert(2, box);
  PASS;
  TEST("TESTING COLLISION OF BOXES WITHIN OCTREE")
  box.minx =  2.5;
  box.miny =  2.5;
  box.minz =  2.5;
  box.maxx =  3.5;
  box.maxy =  3.5;
  box.maxz =  3.5;
  std::set<int> collisions = t.get_collisions(box);
  if((collisions.size() == 1) && (collisions.find(2) != collisions.end()))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING LAZY UPDATE");
  box.minx =  3.3;
  box.miny =  3.3;
  box.minz =  3.3;
  box.maxx =  6;
  box.maxy =  6;
  box.maxz =  6;
  t.insert(3, box);
  PASS;
  TEST("TESTING LAZY QUERY");
  box.minx =  0;
  box.miny =  0;
  box.minz =  0;
  box.maxx =  50;
  box.maxy =  50;
  box.maxz =  50;
  collisions = t.get_collisions(box);
  if(collisions.size() == 3)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING PERFORMANCE OF OCTREE");
  box.minx =  -1;
  box.miny =  -7;
  box.minz =  -3;
  box.maxx =  2;
  box.maxy =  6;
  box.maxz =  11;
  t.insert(4, box);
  box.minx =  -15;
  box.miny =  -17;
  box.minz =  -23;
  box.maxx =  -2;
  box.maxy =  -6;
  box.maxz =  -11;
  t.insert(5, box);
  box.minx =  3;
  box.miny =  5;
  box.minz =  7;
  box.maxx =  11;
  box.maxy =  13;
  box.maxz =  17;
  t.insert(6, box);
  box.minx =  23;
  box.miny =  34;
  box.minz =  45;
  box.maxx =  56;
  box.maxy =  67;
  box.maxz =  78;
  t.insert(7, box);
  box.minx =  -1;
  box.miny =  -2;
  box.minz =  -3;
  box.maxx =  2;
  box.maxy =  3;
  box.maxz =  4;
  t.insert(8, box);
  box.minx =  -100;
  box.miny =  -100;
  box.minz =  -100;
  box.maxx =  100;
  box.maxy =  100;
  box.maxz =  100;
  t.insert(9, box);
  box.minx =  78;
  box.miny =  78;
  box.minz =  78;
  box.maxx =  99;
  box.maxy =  99;
  box.maxz =  99;
  t.insert(10, box);
  box.minx =  3;
  box.miny =  5;
  box.minz =  7;
  box.maxx =  9;
  box.maxy =  11;
  box.maxz =  13;
  t.get_collisions(box);
  t.insert(12, box);
  box.minx =  -5;
  box.miny =  -2;
  box.minz =  7;
  box.maxx =  -2;
  box.maxy =  2;
  box.maxz =  11;
  t.get_collisions(box);
  t.insert(15, box);
  box.minx =  0;
  box.miny =  -250;
  box.minz =  250;
  box.maxx =  1000;
  box.maxy =  1000;
  box.maxz =  1000;
  t.get_collisions(box);
  PASS;
}

int main()
{
  test_octree();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

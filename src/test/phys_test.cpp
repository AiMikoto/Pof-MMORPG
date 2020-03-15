#include "phys/octree.h"
#include "phys/slicing.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "lib/log.h"

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;

int failures;
int tests;

void test_octree()
{
  octree t(root_aabb());
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

graphics::Mesh *mesh_generator()
{
  std::vector<uint> vec;
  graphics::Vertex v1;
  graphics::Vertex v2;
  v1.position = {-1, -1, -1};
  v2.position = {1, 1, 1};
  std::vector<graphics::Vertex> vertices;
  vertices.push_back(v1);
  vertices.push_back(v2);
  return new graphics::Mesh(vertices, vec, vec);
}

bool equals(double a, double b)
{
  return std::abs(a - b) < 0.01;
}

bool equals(glm::dvec3 a, glm::dvec3 b)
{
  return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z);
}

void test_aabb()
{
  TEST("TESTING AABB SIZE");
  graphics::Mesh *m = mesh_generator();
  m -> transform.position = {4, 6, 10};
  container *c = new container(m, box, true, true);
  aabb box = c -> to_aabb();
  if(equals(box.minx, 3)
  && equals(box.maxx, 5)
  && equals(box.miny, 5)
  && equals(box.maxy, 7)
  && equals(box.minz, 9)
  && equals(box.maxz, 11)
    )
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  // delete c;
}

void test_slicing()
{
  TEST("TESTING TICKING OF EMPTY ENVIRONMENT");
  environment *e = new environment();
  tick(e);
  PASS;
  TEST("TESTING TICKING OF STATIC OBJECT");
  graphics::Mesh *m = mesh_generator();
  m -> transform.position = {0, 0, 0};
  container *c = new container(m, box, false, true);
  e -> add(c);
  tick(e);
  bool velo_equals = equals(c -> velocity, {0, 0, 0});
  bool pos_equals = equals(c -> o -> transform.position, {0, 0, 0});
  if(velo_equals && pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING TICKING OF MOVABLE OBJECT");
  m = mesh_generator();
  m -> transform.position = {0, 100, 0};
  c = new container(m, box, true, true);
  e -> add(c);
  tick(e);
  velo_equals = equals(c -> velocity, {0, -1, 0});
  pos_equals = equals(c -> o -> transform.position, {0, 99.5, 0});
  if(velo_equals && pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING FALLING OBJECT EVENTUALLY RESTING ON SURFACE");
  tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(e)))))))))))));
  velo_equals = equals(c -> velocity, {0, 0, 0});
  if(velo_equals && pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  pos_equals = equals(c -> o -> transform.position, {0, 1, 0});
  // delete e;
}

int main()
{
  log_test();
  test_octree();
  test_aabb();
  test_slicing();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

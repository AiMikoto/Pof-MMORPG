#include "phys/octree.h"
#include "phys/collisions.h"
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
  container *c = new container(m, box, true, true, true);
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

void test_collisions()
{
  graphics::Mesh *m1 = mesh_generator();
  m1 -> transform.position = {10, 10, 10};
  m1 -> transform.scale = {3, 3, 3};
  container *c1 = new container(m1, box, true, true, true);
  // obj1 is 7 -> 13
  graphics::Mesh *m2 = mesh_generator();
  container *c2 = new container(m2, box, true, true, true);
  TEST("TESTING BOX-BOX COLLISION NESTED");
  m2 -> transform.position = {10, 10, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING SURFACE");
  m2 -> transform.position = {10, 10, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING EDGE");
  m2 -> transform.position = {10, 12, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING CORNER");
  m2 -> transform.position = {12, 12, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION SURFACE PEEKING OUT");
  m2 -> transform.position = {10, 10, 12.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -X");
  m2 -> transform.position = {6, 10, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +X");
  m2 -> transform.position = {14, 10, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -Y");
  m2 -> transform.position = {10, 6, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Y");
  m2 -> transform.position = {10, 14, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -Z");
  m2 -> transform.position = {10, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Z");
  m2 -> transform.position = {10, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION SHIFTED SURFACE +Z");
  m2 -> transform.position = {12, 12, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Z");
  m2 -> transform.position = {13, 13, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {10, 6, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {6, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {6, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {14, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {14, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  m2 -> transform.position = {6, 6, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER");
  m2 -> transform.position = {6, 6, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER");
  m2 -> transform.position = {14, 14, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {0, 0, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {0, 100, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {100, 0, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {100, 100, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {0, 100, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {100, 0, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  m2 -> transform.position = {100, 100, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  m2 -> transform.position = {7, 7, 7};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  m2 -> transform.position = {13.5, 13.5, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  m2 -> transform.position = {7, 13.5, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  m2 -> transform.position = {13.5, 7, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED NESTED");
  m2 -> transform.position = {10, 10, 10};
  m2 -> transform.rotateTo(1, {2, 3, 4});
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  m2 -> transform.rotateTo(pi/4, {0, 1, 0});
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE PEEKING");
  m2 -> transform.position = {10, 13.5, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE TOUCHING");
  m2 -> transform.position = {10, 14, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE DETACHED");
  m2 -> transform.position = {10, 14.5, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  double sq2 = 2;
  sq2 = std::sqrt(sq2);
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE INSIDE SURFACE");
  m2 -> transform.position = {10 + 3 + sq2 - 0.1, 10, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE OUTSIDE SURFACE");
  m2 -> transform.position = {10 + 3 + sq2 + 0.1, 10, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  double pos = 10 + 3 + sq2 / 2;
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE INSIDE EDGE");
  m2 -> transform.position = {pos - 0.1, 10, pos - 0.1};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE OUTSIDE EDGE");
  m2 -> transform.position = {pos + 0.1, 10, pos + 0.1};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  graphics::Mesh *m3 = mesh_generator();
  m3 -> transform.position = {10, 10, 10};
  m3 -> transform.scale = {1, 6, 1};
  container *c3 = new container(m3, caps, true, true, true);
  graphics::Mesh *m4 = mesh_generator();
  m4 -> transform.scale = {1, 6, 1};
  container *c4 = new container(m4, caps, true, true, true);
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  m4 -> transform.position = {8, 10, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  m4 -> transform.position = {12, 10, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  m4 -> transform.position = {10, 10, 8};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  m4 -> transform.position = {10, 10, 12};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION NESTED");
  m4 -> transform.position = {10, 8, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION NESTED");
  m4 -> transform.position = {10, 12, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  m4 -> transform.position = {10, 17, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  m4 -> transform.position = {10, 21, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  m4 -> transform.position = {10, 22, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  m4 -> transform.position = {10, 3, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  m4 -> transform.position = {10, -1, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  m4 -> transform.position = {10, -2, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  // gonna be lazy and treat c4 as a box
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  m4 -> transform.position = {8, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  m4 -> transform.position = {12, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  m4 -> transform.position = {10, 10, 12};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  m4 -> transform.position = {10, 10, 8};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION NESTED");
  m4 -> transform.position = {10, 8, 10};
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX COLLISION NESTED");
  m4 -> transform.position = {10, 12, 10};
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX ROTATED NESTED");
  m4 -> transform.rotateTo(1, {2, 3, 4});
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX ROTATED TOUCHING EDGE");
  m4 -> transform.rotateTo(pi/4, {0, 1, 0});
  m4 -> transform.position = {10 - 1 - sq2 - 0.001, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX ROTATED TOUCHING TANGENT");
  m4 -> transform.position = {10 - sq2 - 0.001, 10, 10 - sq2 - 0.001};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  // delete c1;
  // delete c2;
  // delete c3;
  // delete c4;
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
  m -> transform.scale = {10, 10, 10};
  container *c = new container(m, box, false, true, false);
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
  TEST("TESTING TICKING OF STILL MOVABLE OBJECT");
  m = mesh_generator();
  m -> transform.position = {0, 11, 0};
  c = new container(m, box, true, false, true);
  e -> add(c);
  int ticks = 300;
  while(ticks--)
  {
    tick(e);
  }
  pos_equals = equals(c -> o -> transform.position, {0, 11, 0});
  if(pos_equals)
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
  c = new container(m, box, true, false, true);
  e -> add(c);
  tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(e))))))))));
  velo_equals = equals(c -> velocity, {0, 0, 0});
  pos_equals = equals(c -> o -> transform.position, {0, 100, 0});
  if(velo_equals || pos_equals)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING FALLING OBJECT EVENTUALLY RESTING ON SURFACE");
  ticks = 10000;
  while(ticks--)
  {
    tick(e);
  }
  pos_equals = equals(c -> o -> transform.position, {0, 11, 0});
  if(pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  pos_equals = equals(c -> o -> transform.position, {0, 1, 0});
  // delete e;
  TEST("TESTING THROW ONTO CORNER WILL EVENTUALLY SETTLE");
  e = new environment();
  m = mesh_generator();
  m -> transform.position = {0, 0, 0};
  m -> transform.scale = {300, 10, 300};
  c = new container(m, box, false, true, false);
  e -> add(c);
  m = mesh_generator();
  m -> transform.position = {0, 0, 0};
  m -> transform.scale = {10, 300, 300};
  c = new container(m, box, false, true, false);
  e -> add(c);
  m = mesh_generator();
  m -> transform.position = {20, 20, 20};
  c = new container(m, box, true, false, true);
  c -> velocity = {-10, 0, -10};
  e -> add(c);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  bool xeq = equals(c -> o -> transform.position.x, 11);
  bool yeq = equals(c -> o -> transform.position.y, 11);
  if(xeq && yeq)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING THROW ONTO CORNER WILL EVENTUALLY FALL OFF AGAIN");
  ticks = 10000;
  while(ticks--)
  {
    tick(e);
  }
  velo_equals = equals(c -> velocity, {0, 0, -10});
  yeq = equals(c -> o -> transform.position.y, 11);
  if(velo_equals || yeq)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  // delete e;
}

int main()
{
  log_test();
  test_octree();
  test_aabb();
  test_collisions();
  test_slicing();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

#include "phys/octree.h"
#include "phys/collisions.h"
#include "phys/slicing.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "lib/log.h"
#include <cmath>
#include "components/solid_object.h"
#include "components/phys_collider.h"

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
  std::set<unsigned long long> collisions = t.get_collisions(box);
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
  TEST("TESTING OCTREE REMOVAL");
  t.erase(2);
  collisions = t.get_collisions(box);
  if(collisions.size() == 2)
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

bool equals(double a, double b)
{
  return std::abs(a - b) < 0.01;
}

bool equals(glm::dvec3 a, glm::dvec3 b)
{
  return equals(a.x, b.x) && equals(a.y, b.y) && equals(a.z, b.z);
}

engine::GameObject *game_object_generator()
{
  engine::GameObject *go = new engine::GameObject();
  return go;
}

glm::dvec3 standard_size = {2, 2, 2};

void test_aabb()
{
  TEST("TESTING AABB SIZE");
  engine::GameObject *go = game_object_generator();
  go -> transform.position = {4, 6, 10};
  collider *c = new physical_collider(standard_size, box);
  go -> addComponent(c);
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
  delete go;
}

void test_collisions()
{
  engine::GameObject *go1 = game_object_generator();
  go1 -> transform.position = {10, 10, 10};
  go1 -> transform.scale = {3, 3, 3};
  collider *c1 = new physical_collider(standard_size, box);
  go1 -> addComponent(c1);
  // obj1 is 7 -> 13
  engine::GameObject *go2 = game_object_generator();
  collider *c2 = new physical_collider(standard_size, box);
  go2 -> addComponent(c2);
  TEST("TESTING BOX-BOX COLLISION NESTED");
  go2 -> transform.position = {10, 10, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING SURFACE");
  go2 -> transform.position = {10, 10, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING EDGE");
  go2 -> transform.position = {10, 12, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION NESTED TOUCHING CORNER");
  go2 -> transform.position = {12, 12, 12};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION SURFACE PEEKING OUT");
  go2 -> transform.position = {10, 10, 12.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -X");
  go2 -> transform.position = {6, 10, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +X");
  go2 -> transform.position = {14, 10, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -Y");
  go2 -> transform.position = {10, 6, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Y");
  go2 -> transform.position = {10, 14, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE -Z");
  go2 -> transform.position = {10, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Z");
  go2 -> transform.position = {10, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION SHIFTED SURFACE +Z");
  go2 -> transform.position = {12, 12, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING SURFACE +Z");
  go2 -> transform.position = {13, 13, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {10, 6, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {6, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {6, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {14, 10, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {14, 10, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION TOUCHING EDGE");
  go2 -> transform.position = {6, 6, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER");
  go2 -> transform.position = {6, 6, 6};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER");
  go2 -> transform.position = {14, 14, 14};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {0, 0, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {0, 100, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {100, 0, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {100, 100, 0};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {0, 100, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {100, 0, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION BIGDISTANCE");
  go2 -> transform.position = {100, 100, 100};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  go2 -> transform.position = {7, 7, 7};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  go2 -> transform.position = {13.5, 13.5, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  go2 -> transform.position = {7, 13.5, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION CORNER COLLISION");
  go2 -> transform.position = {13.5, 7, 13.5};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED NESTED");
  go2 -> transform.position = {10, 10, 10};
  go2 -> transform.rotateTo(1, {2, 3, 4});
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  go2 -> transform.rotateTo(pi/4, {0, 1, 0});
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE PEEKING");
  go2 -> transform.position = {10, 13.5, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE TOUCHING");
  go2 -> transform.position = {10, 14, 10};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED SURFACE DETACHED");
  go2 -> transform.position = {10, 14.5, 10};
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
  go2 -> transform.position = {10 + 3 + sq2 - 0.1, 10, 10};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE OUTSIDE SURFACE");
  go2 -> transform.position = {10 + 3 + sq2 + 0.1, 10, 10};
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
  go2 -> transform.position = {pos - 0.1, 10, pos - 0.1};
  if(box_box(c1, c2))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING BOX-BOX COLLISION ROTATED EDGE OUTSIDE EDGE");
  go2 -> transform.position = {pos + 0.1, 10, pos + 0.1};
  if(box_box(c1, c2))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  engine::GameObject *go3 = game_object_generator();
  go3 -> transform.position = {10, 10, 10};
  go3 -> transform.scale = {1, 6, 1};
  collider *c3 = new physical_collider(standard_size, caps);
  go3 -> addComponent(c3);
  engine::GameObject *go4 = game_object_generator();
  go4 -> transform.scale = {1, 6, 1};
  collider *c4 = new physical_collider(standard_size, caps);
  go4 -> addComponent(c4);
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  go4 -> transform.position = {8, 10, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  go4 -> transform.position = {12, 10, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  go4 -> transform.position = {10, 10, 8};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION PARALLEL");
  go4 -> transform.position = {10, 10, 12};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION NESTED");
  go4 -> transform.position = {10, 8, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION NESTED");
  go4 -> transform.position = {10, 12, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  go4 -> transform.position = {10, 17, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  go4 -> transform.position = {10, 21, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION TOP");
  go4 -> transform.position = {10, 22, 10};
  if(capsule_capsule(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  go4 -> transform.position = {10, 3, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  go4 -> transform.position = {10, -1, 10};
  if(capsule_capsule(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE CAPSULE COLLISION BOTTOM");
  go4 -> transform.position = {10, -2, 10};
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
  go4 -> transform.position = {8, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  go4 -> transform.position = {12, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  go4 -> transform.position = {10, 10, 12};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION PARALLEL");
  go4 -> transform.position = {10, 10, 8};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX COLLISION NESTED");
  go4 -> transform.position = {10, 8, 10};
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX COLLISION NESTED");
  go4 -> transform.position = {10, 12, 10};
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX ROTATED NESTED");
  go4 -> transform.rotateTo(1, {2, 3, 4});
  if(capsule_box(c3, c4))
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING CAPSULE BOX ROTATED TOUCHING EDGE");
  go4 -> transform.rotateTo(pi/4, {0, 1, 0});
  go4 -> transform.position = {10 - 1 - sq2 - 0.001, 10, 10};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING CAPSULE BOX ROTATED TOUCHING TANGENT");
  go4 -> transform.position = {10 - sq2 - 0.001, 10, 10 - sq2 - 0.001};
  if(capsule_box(c3, c4))
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  delete go3;
  delete go4;
  delete go1;
  delete go2;
}

void test_slicing()
{
  TEST("TESTING TICKING OF EMPTY ENVIRONMENT");
  engine::Scene *e = new engine::Scene();
  tick(e);
  PASS;
  TEST("TESTING TICKING OF STATIC OBJECT");
  engine::GameObject *go = game_object_generator();
  go -> transform.position = {0, 0, 0};
  go -> transform.scale = {10, 10, 10};
  collider *c = new physical_collider(standard_size, box);
  go -> addComponent(c);
  e -> addGameObject(go);
  tick(e);
  bool pos_equals = equals(go -> transform.position, {0, 0, 0});
  if(pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING TICKING OF STILL MOVABLE OBJECT");
  go = game_object_generator();
  go -> transform.position = {0, 12, 0};
  go -> transform.scale = {1, 2, 1};
  c = new physical_collider(standard_size, caps);
  go -> addComponent(c);
  solid_object *gop = new solid_object();
  go -> addComponent(gop);
  e -> addGameObject(go);
  int ticks = 300;
  while(ticks--)
  {
    tick(e);
  }
  pos_equals = equals(go -> transform.position, {0, 12, 0});
  if(pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING TICKING OF MOVABLE OBJECT");
  go = game_object_generator();
  go -> transform.position = {0, 100, 0};
  go -> transform.scale = {1, 2, 1};
  c = new physical_collider(standard_size, caps);
  go -> addComponent(c);
  gop = new solid_object();
  go -> addComponent(gop);
  e -> addGameObject(go);
  tick(tick(tick(tick(tick(tick(tick(tick(tick(tick(e))))))))));
  bool velo_equals = equals(gop -> velocity, {0, 0, 0});
  pos_equals = equals(go -> transform.position, {0, 100, 0});
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
  pos_equals = equals(go -> transform.position, {0, 12, 0});
  if(pos_equals)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  delete e;
  TEST("TESTING THROW ONTO CORNER WILL EVENTUALLY SETTLE");
  e = new engine::Scene();
  go = game_object_generator();
  go -> transform.position = {0, 0, 0};
  go -> transform.scale = {300, 10, 300};
  c = new physical_collider(standard_size, box);
  go -> addComponent(c);
  e -> addGameObject(go);
  go = game_object_generator();
  go -> transform.position = {0, 0, 0};
  go -> transform.scale = {10, 300, 300};
  c = new physical_collider(standard_size, box);
  go -> addComponent(c);
  e -> addGameObject(go);
  go = game_object_generator();
  go -> transform.position = {-20, 20, 20};
  go -> transform.scale = {1, 2, 1};
  c = new physical_collider(standard_size, box);
  go -> addComponent(c);
  gop = new solid_object();
  gop -> velocity = {10, 0, -10};
  go -> addComponent(gop);
  e -> addGameObject(go);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  bool xeq = equals(go -> transform.position.x, -11);
  bool yeq = equals(go -> transform.position.y, 12);
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
  velo_equals = equals(gop -> velocity, {0, 0, -10});
  yeq = equals(go -> transform.position.y, 11);
  if(velo_equals || yeq)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  delete e;
}

void test_injector()
{
  int ticks;
  engine::Scene *e = new engine::Scene();
  oid_t oid;
  engine::GameObject *o;
  TEST("TESTING OBJECT INJECTION");
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  oid.at(1);
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  PASS;
  TEST("TESTING OBJECT INJECTION OVERWRITING ANOTHER OBJECT");
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  PASS;
  TEST("TESTING OBJECT INJECTION ONTO THE SAME OBJECT");
  o = new engine::GameObject();
  oid.pop();
  oid.at(oid.add(e, o));
  slicer_inject_object(oid, o);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  PASS;
  TEST("TESTING INJECTED SCENE DELETION");
  delete e;
  PASS;
  TEST("TESTING DOUBLE OBJECT INJECTION");
  e = new engine::Scene();
  oid = oid_t();
  oid.at(1); // [1]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  oid.pop();
  oid.at(2); // [2]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  PASS;
  /*    e
   *   /|
   *  1 2
   */
  TEST("TESTING CHILD OBJECT INJECTION");
  oid.at(1); // [2, 1]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  if(oid.get(e) != NULL)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  boost::property_tree::ptree sast = e -> serialize();
  TEST("TESTING CHILD MOVEMENT OVERWRITE");
  oid_t from, to;
  from = oid; // [2, 1]
  to.at(1); // [1]
  engine::GameObject *go = from.get(e);
  slicer_eject_object(from);
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  /* yes, [1] gets overwritten by [2, 1]
   *    e
   *   /|
   *  1 2
   */
  TEST("TESTING CHILD MOVEMENT OVERWRITE 2");
  from.pop(); // [2]
  go = from.get(e);
  slicer_eject_object(from);
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
  TEST("TESTING CHILD NORMAL MOVEMENT");
  e = new engine::Scene(sast);
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  from.at(1); // [2, 1]
  go = from.get(e);
  slicer_eject_object(from);
  to.at(to.add(e, new engine::GameObject())); // [1, 1]
  // new object will be deleted by slicer;
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
  TEST("TESTING CHILD DELETION");
  e = new engine::Scene(sast);
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  oid = oid_t();
  oid.at(2); // [2]
  oid.at(1); // [2, 1]
  slicer_eject_object(oid);
  ticks = 1000;
  while(ticks--)
  {
    tick(e);
  }
  if((oid.get(e) == NULL) && (oid.pop() -> get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
}

void test_slice_serialisation()
{
  int ticks;
  engine::Scene *e = new engine::Scene();
  oid_t oid;
  engine::GameObject *o;
  TEST("TESTING OBJECT INJECTION - SERIAL");
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  oid.at(1);
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  PASS;
  TEST("TESTING OBJECT INJECTION OVERWRITING ANOTHER OBJECT - SERIAL");
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  PASS;
  TEST("TESTING OBJECT INJECTION ONTO THE SAME OBJECT - SERIAL");
  o = new engine::GameObject();
  oid.pop();
  oid.at(oid.add(e, o));
  slicer_inject_object(oid, o);
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  PASS;
  TEST("TESTING INJECTED SCENE DELETION - SERIAL");
  delete e;
  PASS;
  TEST("TESTING DOUBLE OBJECT INJECTION - SERIAL");
  e = new engine::Scene();
  oid = oid_t();
  oid.at(1); // [1]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  oid.pop();
  oid.at(2); // [2]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  PASS;
  /*    e
   *   /|
   *  1 2
   */
  TEST("TESTING CHILD OBJECT INJECTION - SERIAL");
  oid.at(1); // [2, 1]
  o = new engine::GameObject();
  slicer_inject_object(oid, o);
  delete o;
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  if(oid.get(e) != NULL)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  boost::property_tree::ptree sast = e -> serialize();
  TEST("TESTING CHILD MOVEMENT OVERWRITE - SERIAL");
  oid_t from, to;
  from = oid; // [2, 1]
  to.at(1); // [1]
  engine::GameObject *go = from.get(e);
  slicer_eject_object(from);
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  /* yes, [1] gets overwritten by [2, 1]
   *    e
   *   /|
   *  1 2
   */
  TEST("TESTING CHILD MOVEMENT OVERWRITE 2 - SERIAL");
  from.pop(); // [2]
  go = from.get(e);
  slicer_eject_object(from);
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
  TEST("TESTING CHILD NORMAL MOVEMENT - SERIAL");
  e = new engine::Scene(sast);
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  from.at(1); // [2, 1]
  go = from.get(e);
  slicer_eject_object(from);
  to.at(to.add(e, new engine::GameObject())); // [1, 1]
  // new object will be deleted by slicer;
  slicer_inject_object(to, go);
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  if((from.get(e) == NULL) && (to.get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
  TEST("TESTING CHILD DELETION - SERIAL");
  e = new engine::Scene(sast);
  /*    e
   *   /|
   *  1 2
   *    |
   *    1
   */
  oid = oid_t();
  oid.at(2); // [2]
  oid.at(1); // [2, 1]
  slicer_eject_object(oid);
  ticks = 1000;
  while(ticks--)
  {
    apply_slice(e, slice_t(slice(e).encode()));
  }
  if((oid.get(e) == NULL) && (oid.pop() -> get(e) != NULL))
  {
    PASS
  }
  else
  {
    FAIL
  }
  delete e;
}

int main()
{
  log_test();
  test_octree();
  test_aabb();
  test_collisions();
  test_slicing();
  test_injector();
  test_slice_serialisation();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

#include "scene/scene.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;
#define T_EQ(s1, s2) if(scene_eq(s1, s2)) {PASS;} else {FAIL;}

bool col_eq(collider *c1, collider *c2)
{
  if(c1 == c2)
  { // same memory location
    return true;
  }
  if((c1 == NULL) || (c2 == NULL))
  {
    return false;
  }
  return c1 -> c_type == c2 -> c_type && c1 -> size == c2 -> size;
}

bool so_eq(solid_object *c1, solid_object *c2)
{
  if(c1 == c2)
  { // same memory location
    return true;
  }
  if((c1 == NULL) || (c2 == NULL))
  {
    return false;
  }
  return c1 -> m == c2 -> m && c1 -> im == c2 -> im && c1 -> velocity == c2 -> velocity;
}

bool obj_eq(engine::GameObject *o1, engine::GameObject *o2)
{
  bool assume = true;
  assume = assume && o1 -> name               == o2 -> name;
  assume = assume && o1 -> tag                == o2 -> tag;
  assume = assume && o1 -> transform.position == o2 -> transform.position;
  assume = assume && o1 -> transform.rotation == o2 -> transform.rotation;
  assume = assume && o1 -> transform.scale    == o2 -> transform.scale;
  assume = assume && col_eq(o1 -> getComponent<physical_collider>(), o2 -> getComponent<physical_collider>());
  assume = assume && so_eq (o1 -> getComponent<solid_object>(), o2 -> getComponent<solid_object>());
  return assume;
}

bool scene_eq(engine::Scene *s1, engine::Scene *s2)
{
  bool assume = true;
  for (auto g : s1 -> gameObjects) {
    assume = assume && s2 -> gameObjects.find(g.first) != s2 -> gameObjects.end() && obj_eq(g.second, s2 -> gameObjects[g.first]);
  }
  return assume;
}

int failures;
int tests;

void test_serialisation()
{
  engine::Scene *s1, *s2;
  engine::GameObject *o;
  engine::Component *c;
  TEST("TESTING SCENE EQUALITY OF EMPTY SCENE")
  s1 = new engine::Scene();
  s2 = new engine::Scene(s1 -> serialize());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OBJECT")
  s1 = new engine::Scene();
  s1 -> addGameObject(new engine::GameObject());
  s2 = new engine::Scene(s1 -> serialize());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OFFSET OBJECT")
  s1 = new engine::Scene();
  o = new engine::GameObject();
  o -> tag = "asd";
  s1 -> addGameObject(o);
  s2 = new engine::Scene(s1 -> serialize());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OFFSET OBJECT VIA JSON")
  s1 = new engine::Scene();
  o = new engine::GameObject();
  o -> tag = "asd";
  s1 -> addGameObject(o);
  s2 = new engine::Scene();
  s2 -> fromJSON(s1 -> toJSON());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OFFSET OBJECT VIA FILE")
  s1 = new engine::Scene();
  o = new engine::GameObject();
  o -> tag = "asd";
  s1 -> addGameObject(o);
  s2 = new engine::Scene();
  s1 -> writeToFile("test.json");
  s2 -> readFromFile("test.json");
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OBJECT WITH COMPONENTS")
  s1 = new engine::Scene();
  o = new engine::GameObject();
  o -> tag = "asd";
  c = new physical_collider({1, 1, 1}, caps);
  o -> addComponent(c);
  c = new solid_object();
  ((solid_object *)c) -> velocity = {10, -3, 7.2};
  o -> addComponent(c);
  s1 -> addGameObject(o);
  s2 = new engine::Scene(s1 -> serialize());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
}

int main()
{
  test_serialisation();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

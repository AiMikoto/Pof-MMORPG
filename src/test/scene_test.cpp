#include "scene/scene.h"
#include <cstdio>
#include <iostream>
#include <string>

#define TEST(x) printf(x " - ");tests++;
#define PASS printf("PASSED\n");
#define FAIL printf("FAILED\n");failures++;
#define T_EQ(s1, s2) if(scene_eq(s1, s2)) {PASS;} else {FAIL;}

bool obj_eq(engine::GameObject *o1, engine::GameObject *o2)
{
  bool assume = true;
  assume = assume && o1 -> name               == o2 -> name;
  assume = assume && o1 -> tag                == o2 -> tag;
  assume = assume && o1 -> transform.position == o2 -> transform.position;
  assume = assume && o1 -> transform.rotation == o2 -> transform.rotation;
  assume = assume && o1 -> transform.scale    == o2 -> transform.scale;
  assume = assume && o1 -> id                 == o2 -> id;
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
  o -> id = 7;
  o -> tag = "asd";
  s1 -> addGameObject(o);
  s2 = new engine::Scene(s1 -> serialize());
  T_EQ(s1, s2);
  delete s1;
  delete s2;
  TEST("TESTING SCENE EQUALITY OF ONE OFFSET OBJECT VIA JSON")
  s1 = new engine::Scene();
  o = new engine::GameObject();
  o -> id = 7;
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
  o -> id = 7;
  o -> tag = "asd";
  s1 -> addGameObject(o);
  s2 = new engine::Scene();
  s1 -> writeToFile("test.json");
  s2 -> readFromFile("test.json");
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

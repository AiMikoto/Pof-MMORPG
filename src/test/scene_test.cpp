#include "scene/scene.h"
#include "components/phys_collider.h"
#include "components/solid_object.h"
#include <cstdio>
#include <iostream>
#include <string>
#include "scene/oid.h"

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
  for (auto g : s1 -> children) {
    assume = assume && s2 -> children.find(g.first) != s2 -> children.end() && obj_eq(g.second, s2 -> children[g.first]);
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
  s1 -> children[7] = o;
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

void test_oid()
{
  engine::Scene *s = new engine::Scene();
  TEST("TESTING OID EQUALITY");
  oid_t oid, oid2;
  oid.at(1);
  oid.at(3);
  oid.at(2);
  oid2.at(1);
  oid2.at(3);
  oid2.at(2);
  if(oid == oid2)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  oid2.at(1);
  TEST("TESTING OID INEQUALITY");
  if(oid == oid2)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING OID ASSIGNMENT");
  oid2 = oid;
  if(oid == oid2)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING OID SEPARATION");
  oid.pop();
  if(oid == oid2)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  oid = oid_t();
  oid2 = oid_t();
  TEST("GETTING INEXISTENT ID");
  oid.at(2);
  if(oid.get(s) != NULL)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("GETTING EMPTY ID");
  if(oid2.get(s) != s)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("GETTING ADDITION");
  engine::GameObject *go = new engine::GameObject();
  oid.put(s, go);
  if(oid.get(s) != go)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("GETTING INSERTION");
  oid2 = oid;
  oid2.at(1);
  go = new engine::GameObject();
  oid.add(s, go);
  if(oid2.get(s) != go)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING ANOTHER INSERTION");
  go = new engine::GameObject();
  oid.add(s, go);
  oid2.pop();
  oid2.at(2);
  if(oid2.get(s) != go)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING OID ENCODING/DECODING");
  oid = oid_t(oid2.encode());
  if(oid == oid2)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  // let's clean up
  delete s;
  s = new engine::Scene();
  go = new engine::GameObject();
  TEST("TESTING INVALID OBJECT ADDITION");
  oid = oid_t();
  oid2 = oid_t();
  if(oid2.put(s, go) == 0)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING INVALID OBJECT INJECTION");
  oid2.at(3);
  oid2.at(7);
  if(oid2.put(s, go) == 0)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING INVALID OBJECT INJECTION");
  oid2.pop();
  if(oid2.add(s, go) == 0)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING ADVANCED TREE CONSTRUCTION");
  // let's construct a somewhat advanced tree
  oid_t oid3; // []
  oid3.add(go, new engine::GameObject());
  oid3.at(3); // [3]
  oid3.put(s, go);
  /*   s
   *  /|
   * 1 3
   */
  oid3.pop(); // []
  oid3.add(go, new engine::GameObject());
  oid3.add(go, new engine::GameObject());
  oid3.at(99); // [99]
  oid3.put(go, new engine::GameObject());
  /*   s
   *  /|
   * 1 3
   *  /|\
   * 1 2 99
   */
  oid3.add(go, new engine::GameObject());
  /*   s
   *  /|
   * 1 3
   *  /|\
   * 1 2 99
   *     |
   *     1
   */
  PASS
  TEST("TESTING SOMEWHAT ADVANCED ACCESS");
  oid = oid_t();
  if((&oid) -> at(3) -> at(99) -> at(1) -> get(s) == NULL)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING MORE ADVANCED ACCESS");
  oid = oid_t();
  if((&oid) -> at(3) -> at(99) -> at(1) -> pop() -> pop() -> at(2) -> get(s) == NULL)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING ADVANCED INVALID ACCESS");
  oid = oid_t();
  if((&oid) -> at(3) -> at(2) -> at(7) -> get(s) == NULL)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING ADVANCED INJECTION");
  oid = oid_t();
  if((&oid) -> at(3) -> at(2) -> at(7) -> put(s, new engine::GameObject()) == 7)
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING ADVANCED VALID ACCESS");
  oid = oid_t();
  if((&oid) -> at(3) -> at(2) -> at(7) -> get(s) == NULL)
  {
    FAIL;
  }
  else
  {
    PASS;
  }
  TEST("TESTING SERIALISATION");
  oid = oid_t();
  
  if((&oid) -> at(3) -> at(2) -> at(7) -> pop() -> at(13) -> at(91) -> serialise() == "3.2.13.91")
  {
    PASS;
  }
  else
  {
    FAIL;
  }
  TEST("TESTING SERIALISATION");
  oid2 = oid_t(oid.serialise());
  if(oid == oid2)
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
  test_serialisation();
  test_oid();
  printf("PASSED %d/%d TESTS!\n", tests - failures, tests);
  return failures;
}

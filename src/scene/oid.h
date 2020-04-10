#ifndef SCENE_OID_H
#define SCENE_OID_H

#include <boost/property_tree/ptree.hpp>
#include "scene/objects.h"
#include <string>

// used for convenient access and manipulation of scenes and gameobjects.
// see test/scene_test :: test_oid for examples.

class oid_t
{
public:
  oid_t();
  oid_t(boost::property_tree::ptree node);
  oid_t(std::string serial);
  boost::property_tree::ptree encode();
  std::string serialise();
  oid_t *at(unsigned long long id);
  oid_t *pop();
  engine::GameObject *get(engine::GameObject *o);
  unsigned long long put(engine::GameObject *o, engine::GameObject *target);
  unsigned long long add(engine::GameObject *o, engine::GameObject *target);
  bool operator == (const oid_t &other);
private:
  std::vector<unsigned long long> _access_order;
};

#endif

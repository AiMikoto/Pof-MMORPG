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
  boost::property_tree::ptree encode() const;
  std::string serialise() const;
  oid_t *at(unsigned long long id);
  oid_t *pop();
  engine::GameObject *get(engine::GameObject *o) const;
  unsigned long long put(engine::GameObject *o, engine::GameObject *target) const;
  unsigned long long add(engine::GameObject *o, engine::GameObject *target) const;
  unsigned long long destroy(engine::GameObject *o) const;
  bool operator == (const oid_t &other) const;
  bool operator< (const oid_t &other) const;
private:
  std::vector<unsigned long long> _access_order;
};

#endif

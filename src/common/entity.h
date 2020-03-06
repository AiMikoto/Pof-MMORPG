#ifndef COMMON_ENTITY_H
#define COMMON_ENTITY_H

#include "graphics/objects.h"
#include <vector>
#include <string>
#include <boost/property_tree/json_parser.hpp>

#define entity_id int

class entity:graphics::GameObject
{
public:
  entity();
  entity(boost::property_tree::ptree json);
  boost::property_tree::ptree encode();
};

class environment
{
public:
  environment();
  ~environment();
  std::string save();
  void load(std::string encoded);
  void add_entity(entity_id ent_id, entity *ent);
  entity *get(entity_id ent_id);
  void remove_entity(entity_id ent_id);
private:
  std::map<entity_id, entity *> entities;
};

#endif // COMMON_ENTITY_H

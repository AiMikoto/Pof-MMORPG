#include "common/entity.h"

environment::environment()
{
}

environment::~environment()
{
  for(auto it:entities)
  {
    delete it.second;
  }
}

std::string environment::save()
{
  boost::property_tree::ptree tree;
  for(auto it:entities)
  {
    tree.put_child(std::to_string(it.first), it.second -> encode());
  }
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, tree);
  return ss.str();
}

void environment::load(std::string encoded)
{
  boost::property_tree::ptree tree;
  std::stringstream ss(encoded);
  boost::property_tree::json_parser::read_json(ss, tree);
  for(auto it:tree)
  {
    this -> add_entity(std::stoi(it.first), new entity(it.second));
  }
}

void environment::add_entity(entity_id ent_id, entity *ent)
{
  entities[ent_id] = ent;
}

entity *environment::get(entity_id ent_id)
{
  return entities[ent_id];
}

void environment::remove_entity(entity_id ent_id)
{
  entities.erase(ent_id);
}

entity::entity():GameObject()
{
}

entity::entity(boost::property_tree::ptree):GameObject()
{
  // TODO: generate transform from property tree
}

boost::property_tree::ptree entity::encode()
{
  // TODO: generate property tree from transform
  boost::property_tree::ptree tree;
  return tree;
}

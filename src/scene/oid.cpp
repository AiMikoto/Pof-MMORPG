#include "scene/oid.h"
#include <boost/tokenizer.hpp>

oid_t::oid_t()
{
}

oid_t::oid_t(boost::property_tree::ptree node)
{;
  for(auto i : node)
  {
    at(i.second.get<unsigned long long>(""));
  }
}

oid_t::oid_t(std::string serial)
{
  boost::char_separator<char> sep(".");
  boost::tokenizer<boost::char_separator<char>> tokens(serial, sep);
  for(auto t : tokens)
  {
    at(stoull(t));
  }
}

boost::property_tree::ptree oid_t::encode() const
{
  boost::property_tree::ptree node;
  for(auto i : _access_order)
  {
    node.add("aoe", i);
  }
  return node;
}

std::string oid_t::serialise() const
{
  std::string serial = "";
  for(auto i : _access_order)
  {
    serial = serial + std::to_string(i) + '.';
  }
  if(serial.size() > 0)
  {
    serial.pop_back();
  }
  return serial;
}

oid_t *oid_t::at(unsigned long long id)
{
  _access_order.push_back(id);
  return this;
}

oid_t *oid_t::pop()
{
  if(_access_order.size() > 0)
  {
    _access_order.pop_back();
  }
  return this;
}

bool oid_t::is_in(const oid_t &other) const
{ // check if this is a subpath of other
  // for example, 1.2.3 is a subpath of 1.2.3.4.5
  // but 1.2.4 is not a subpath of 1.2.3.4.5
  if(other._access_order.size() < this -> _access_order.size())
  {
    return false;
  }
  for(int i = 0; i < this -> _access_order.size(); i++)
  {
    if(this -> _access_order[i] != other._access_order[i])
    {
      return false;
    }
  }
  return true;
}

engine::GameObject *oid_t::get(engine::GameObject *o) const
{
  engine::GameObject *node = o;
  try
  {
    for(int i = 0; i < _access_order.size(); i++)
    {
      node = node -> at(_access_order[i]);
    }
  }
  catch(std::exception &e)
  {
    node = NULL;
  }
  return node;
}

unsigned long long oid_t::put(engine::GameObject *o, engine::GameObject *target) const
{
  if(_access_order.size() == 0)
  { // use add instead?
    return 0;
  }
  engine::GameObject *node = o;
  try
  {
    for(int i = 0; i < _access_order.size() - 1; i++)
    {
      node = node -> at(_access_order[i]);
    }
    // place at exact id
    return node -> addGameObject(_access_order[_access_order.size() - 1], target);
  }
  catch(std::exception &e)
  {
    return 0;
  }
}

unsigned long long oid_t::add(engine::GameObject *o, engine::GameObject *target) const
{
  engine::GameObject *node = o;
  try
  {
    for(int i = 0; i < _access_order.size(); i++)
    {
      node = node -> at(_access_order[i]);
    }
    return node -> addGameObject(target);
  }
  catch(std::exception &e)
  {
    return 0;
  }
}

unsigned long long oid_t::destroy(engine::GameObject *o) const
{
  if(_access_order.size() == 0)
  { // how is this gonna work?
    return 0;
  }
  engine::GameObject *node = o;
  try
  {
    for(int i = 0; i < _access_order.size() - 1; i++)
    {
      node = node -> at(_access_order[i]);
    }
    // place at exact id
    node -> deleteGameObject(_access_order[_access_order.size() - 1]);
    return 1;
  }
  catch(std::exception &e)
  {
    return 0;
  }
}

bool oid_t::operator== (const oid_t &other) const
{
  return this -> _access_order == other._access_order;
}

bool oid_t::operator< (const oid_t &other) const
{
  return this -> serialise() < other.serialise();
}

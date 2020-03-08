#include "user_card.h"
#include <boost/property_tree/json_parser.hpp>

boost::property_tree::ptree& user_card::tree()
{
  return _tree;
}

void user_card::load(std::string json)
{
  std::stringstream ss(json);
  boost::property_tree::json_parser::read_json(ss, _tree);
}

std::string user_card::save()
{
  std::stringstream ss;
  boost::property_tree::json_parser::write_json(ss, _tree);
  return ss.str();
}

void user_card_library::add(user_card uc)
{
  library[uc.tree().get<std::string>("user.name")] = uc;
}

bool user_card_library::contains(std::string name)
{
  return library.find(name) != library.end();
}

bool user_card_library::contains(user_card uc)
{
  return library.find(uc.tree().get<std::string>("user.name")) != library.end();
}

void user_card_library::remove(std::string name)
{
  if(contains(name))
  {
    library.erase(name);
  }
}

void user_card_library::remove(user_card uc)
{
  if(contains(uc))
  {
    library.erase(uc.tree().get<std::string>("user.name"));
  }
}

user_card& user_card_library::get(std::string name)
{
  return library[name];
}

void user_card_library::apply(applied a)
{
  for(auto it:library)
  {
    a(it.second);
  }
}

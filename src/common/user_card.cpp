#include "user_card.h"

boost::property_tree::ptree& user_card::tree()
{
  return _tree;
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

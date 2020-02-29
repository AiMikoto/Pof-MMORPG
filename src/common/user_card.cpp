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

void user_card_library::remove(std::string name)
{
  library.erase(name);
}

user_card& user_card_library::get(std::string name)
{
  return library[name];
}

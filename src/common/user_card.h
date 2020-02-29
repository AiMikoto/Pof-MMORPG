#ifndef COMMON_USER_CARD_H
#define COMMON_USER_CARD_H

#include <boost/property_tree/ptree.hpp>
#include <map>

class user_card
{
public:
  boost::property_tree::ptree& tree();
private:
  boost::property_tree::ptree _tree;
};

class user_card_library
{
public:
  void add(user_card uc);
  void remove(std::string name);
  bool contains(std::string name);
  user_card& get(std::string name);
private:
  std::map<std::string, user_card> library;
};

#endif // COMMON_USER_CARD_H

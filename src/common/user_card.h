#ifndef COMMON_USER_CARD_H
#define COMMON_USER_CARD_H

#include <boost/property_tree/ptree.hpp>
#include <map>
#include <boost/function.hpp>

class user_card
{
public:
  void load(std::string json);
  std::string save();
  boost::property_tree::ptree& tree();
  void *aux; // hook for whatever you fancy
private:
  boost::property_tree::ptree _tree;
};

typedef boost::function<void(user_card&)> applied;

class user_card_library
{
public:
  void add(user_card uc);
  void remove(std::string name);
  void remove(user_card uc);
  bool contains(std::string name);
  bool contains(user_card uc);
  user_card& get(std::string name);
  void apply(applied a);
private:
  std::map<std::string, user_card> library;
};

#endif // COMMON_USER_CARD_H

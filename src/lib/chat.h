#ifndef LIB_CHAT_H
#define LIB_CHAT_H

#include <deque>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "lib/uuid.h"
#include <boost/assign/list_of.hpp>
#include <boost/unordered_map.hpp>

enum chat_target
{
  ct_local = 0,
  ct_party,
  ct_guild,
  ct_world,
  ct_system
};

const boost::unordered_map<chat_target, std::string> chat_target_as_string = boost::assign::map_list_of
(ct_local, "local")
(ct_party, "party")
(ct_guild, "guild")
(ct_world, "world")
(ct_system, "system");

class message
{
public:
  message(chat_target target, std::string payload);
  message(boost::property_tree::ptree tree);
  boost::property_tree::ptree encode();
  chat_target target;
  std::string payload;
  std::string uuid;
};

class chat_log
{
public:
  void add(message m);
  std::deque<message> get();
  bool flush();
private:
  std::deque<message> chat;
  trie uuid_trie;
  bool dirty = false;
};

#endif // LIB_CHAT_H

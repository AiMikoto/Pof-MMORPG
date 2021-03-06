#ifndef LIB_CHAT_H
#define LIB_CHAT_H

#include <queue>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include "lib/uuid.h"

enum chat_target
{
  local = 0,
  party,
  guild,
  world
};

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
  std::queue<message> get();
private:
  std::queue<message> chat;
  trie uuid_trie;
};

#endif // LIB_CHAT_H

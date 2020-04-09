#include "lib/chat.h"
#include <boost/lexical_cast.hpp>
#include "lib/log.h"

#define CHAT_LIMIT 1000

message::message(chat_target target, std::string payload)
{
  this -> target = target;
  this -> payload = payload;
  this -> uuid = get_uuid();
}

message::message(boost::property_tree::ptree tree)
{
  this -> target = static_cast<chat_target>(tree.get<int>("target"));
  this -> payload = tree.get<std::string>("payload");
  this -> uuid = tree.get<std::string>("uuid");
}

boost::property_tree::ptree message::encode()
{
  boost::property_tree::ptree t;
  t.put("target", target);
  t.put("payload", payload);
  t.put("uuid", uuid);
  return t;
}

void chat_log::add(message m)
{
  if(!uuid_trie.has(m.uuid))
  {
    chat.push_front(m);
    uuid_trie.add(m.uuid);
  }
  if(chat.size() > CHAT_LIMIT)
  {
    uuid_trie.remove(chat.front().uuid);
    chat.pop_back();
  }
}

std::deque<message> chat_log::get()
{
  return chat;
}

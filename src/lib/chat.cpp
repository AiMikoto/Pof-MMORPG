#include "lib/chat.h"

#define CHAT_LIMIT 1000

message::message(chat_target target, std::string payload)
{
  this -> target = target;
  this -> payload = payload;
}

message::message(boost::property_tree::ptree tree)
{
  this -> target = static_cast<chat_target>(tree.get<int>("target"));
  this -> payload = tree.get<std::string>("payload");
}

boost::property_tree::ptree message::encode()
{
  boost::property_tree::ptree t;
  t.put("target", target);
  t.put("payload", payload);
}

void chat_log::add(message m)
{
  chat.push(m);
  if(chat.size() > CHAT_LIMIT)
  {
    chat.pop();
  }
}

std::queue<message> chat_log::get()
{
  return chat;
}

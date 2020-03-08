#include "lib/chat.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "lib/log.h"

#define CHAT_LIMIT 1000

boost::uuids::random_generator msg_generator;

message::message(chat_target target, std::string payload)
{
  this -> target = target;
  this -> payload = payload;
  this -> uuid = boost::lexical_cast<std::string>(msg_generator());
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
  return t;
}

void chat_log::add(message m)
{
  // TODO: verify that the UUID is not already stored
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

#include "chat_server/rooms.h"
#include "lib/log.h"
#include "lib/chat.h"

std::map<std::string, room*> rooms;

room::room(std::string name)
{
  this -> name = name;
  BOOST_LOG_TRIVIAL(trace) << "created room {" << name << "}";
}

void room::send(call c)
{
  for(sub:subs)
  {
    sub -> safe_write(c);
  }
}

void room::subscribe(client *c)
{
  subs.insert(c);
}

void room::unsubscribe(client *c)
{
  subs.erase(c);
  if(subs.size() == 0)
  {
    BOOST_LOG_TRIVIAL(trace) << "removing room {" << name << "}";
    rooms.erase(this -> name);
    delete this;
  }
}

void give_message(std::string target, call c)
{
  if(rooms.find(target) != rooms.end())
  {
    rooms[target] -> send(c);
  }
}

void sub(std::string target, client *c)
{
  if(rooms.find(target) == rooms.end())
  {
    rooms[target] = new room(target);
  }
  rooms[target] -> subscribe(c);
}

void unsub(std::string target, client *c)
{
  if(rooms.find(target) != rooms.end())
  {
    rooms[target] -> unsubscribe(c);
  }
}
#ifndef CHAT_SERVER_ROOMS_H
#define CHAT_SERVER_ROOMS_H

#include "chat_server/client.h"
#include <set>
#include <map>

class room
{
public:
  room(std::string name);
  void send(call c);
  void subscribe(client *c);
  void unsubscribe(client *c);
private:
  std::string name;
  std::set<client *> subs;
};

extern std::map<std::string, room *> rooms;

void give_message(call c);

void sub(std::string target, client *c);

void unsub(std::string target, client *c);

#endif // CHAT_SERVER_ROOMS_H

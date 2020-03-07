#ifndef LIB_CHAT_H
#define LIB_CHAT_H

#include <queue>
#include <string>

enum chat_target
{
  local,
  party,
  guild,
  world
};

class message
{
public:
  message(chat_target target, std::string payload);
  chat_target target = local;
  std::string payload = "";
};

class chat_log
{
public:
  void add(message m);
  std::queue<message> get();
private:
  std::queue<message> chat;
};

#endif // LIB_CHAT_H

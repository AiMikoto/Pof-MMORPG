#ifndef CLIENT_SYSTEM_H
#define CLIENT_SYSTEM_H

#include "lib/chat.h"

class client_system_manager
{
public:
  client_system_manager(chat_log *cl);
  ~client_system_manager();
  void say(std::string msg);
private:
  chat_log *cl;
};

#endif

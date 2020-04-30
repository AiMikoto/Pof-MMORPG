#ifndef CLIENT_SYSTEM_H
#define CLIENT_SYSTEM_H

#include "lib/chat.h"
#include "common/user_card.h"
#include "client/editor.h"

class client_system_manager
{
public:
  client_system_manager();
  ~client_system_manager();
  void say(std::string msg);
  void feed_command(std::string line);
  user_card_library ucl;
  chat_log cl;
private:
  bool try_exit_handler(std::string line);
  bool try_editor_handler(std::string line);
  bool try_viewer_handler(std::string line);
  void handle_linear_input(std::string line);
  editor *e = NULL;
};

extern client_system_manager *csm;

#endif

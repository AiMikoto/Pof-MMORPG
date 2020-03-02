#include "client/game.h"
#include "client/client.h"
#include "lib/log.h"

user_card_library ucl;

void move(std::string host, int port)
{
  init_l.lock();
  init_l.unlock();
  // TODO: put up loading screen
//  g -> unload();
  BOOST_LOG_TRIVIAL(trace) << "closing current instance";
  current_instance -> close();
  delete current_instance;
  // connect to new instance
  BOOST_LOG_TRIVIAL(trace) << "creating new instance";
  current_instance = instance_builder(host, port);
  BOOST_LOG_TRIVIAL(trace) << "attempting to log-in";
  std::string tok = ucl.get(username).tree().get<std::string>("user.token");
  current_instance -> authenticate_token(username, tok);
//  g -> load();
}
#include "client/game.h"
#include "client/client.h"
#include "lib/log.h"

user_card_library ucl;
engine::Scene *current;
std::mutex scene_lock;
chat_log cl;

void move(std::string host, int port)
{
  init_l.lock();
  init_l.unlock();
  // TODO: put up loading screen
  BOOST_LOG_TRIVIAL(trace) << "closing current instance";
  delete current_instance;
  // connect to new instance
  BOOST_LOG_TRIVIAL(trace) << "connecting to new instance";
  current_instance = instance_builder(host, port);
  BOOST_LOG_TRIVIAL(trace) << "attempting to log-in";
  std::string tok = ucl.get(username).tree().get<std::string>("user.token");
  current_instance -> authenticate_token(username, tok);
  // TODO: take down loading screen
  BOOST_LOG_TRIVIAL(trace) << "finished instance movement";
}

void add_slice(slice_t next_slice)
{
  scene_lock.lock();
  apply_slice(current, next_slice);
  scene_lock.unlock();
}

void send_message(chat_target target, std::string payload)
{
  message m(target, payload);
  call c;
  c.tree().put(OPCODE, OP_IRC);
  c.tree().put_child("payload", m.encode());
  current_instance -> safe_write(c);
}

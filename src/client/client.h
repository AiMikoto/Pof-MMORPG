#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "lib/protocol.h"
#include "include/regions.h"
#include "include/maps.h"
#include "lib/chat.h"

void init_crypto(std::string pub);

class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock);
  ~instance();
  bool authenticate(std::string username, std::string password);
  bool authenticate_token(std::string username, std::string tok);
  bool change_map(map_t map, region_t region);
  bool change_map(std::string instance_uuid);
  void send_message(message m);
private:
  void authenticate_cb(std::mutex *lock, bool *status, call c);
  void change_map_cb(std::mutex *lock, bool *status, call c);
  void uc_transfer(call c);
  void move_cb(call c);
  void irc_cb(call c);
};

instance *instance_builder(std::string host, int port);

extern instance *current_instance;

#endif // CLIENT_CLIENT_H

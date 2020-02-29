#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "lib/protocol.h"

class instance:public protocol
{
public:
  instance(boost::asio::ip::tcp::socket *sock);
  ~instance();
  bool authenticate(std::string username, std::string password);
  bool authenticate_token(std::string username, std::string token);
private:
  void authenticate_cb(std::mutex *lock, bool *status, call c);
  void uc_transfer(call c);
  void move_cb(call c);
};

instance *instance_builder(std::string host, int port);

extern instance *current_instance;

#endif // CLIENT_CLIENT_H

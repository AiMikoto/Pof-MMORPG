#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include <boost/asio.hpp>
#include "lib/call.h"

class instance
{
public:
  instance(std::string host, int port);
  ~instance();
  void close();
  void periodic();
private:
  void routine();
  void handle_ping(call c);
  void handle_pong(call c);
  void terminate(call c);
  void terminate_force(call c);
  boost::asio::ip::tcp::socket *socket;
  endpoint_table ept;
};

#endif // CLIENT_CLIENT_H

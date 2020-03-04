#ifndef INSTANCE_SERVER_H
#define INSTANCE_SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include "instance/client.h"

class server
{
public:
  server(int port);
private:
  void routine();
  void cleanup();
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  std::vector<client*> clients;
};

#endif // INSTANCE_SERVER_H

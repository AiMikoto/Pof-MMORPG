#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include "instance/client.h"

class server
{
public:
  server(boost::asio::io_context& ioc, int port);
private:
  void routine();
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::asio::io_context& io_context;
  std::vector<client*> clients;
};

#endif // SERVER_SERVER_H

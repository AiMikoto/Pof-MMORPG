#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include "chat_server/client.h"

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

#endif // CHAT_SERVER_SERVER_H

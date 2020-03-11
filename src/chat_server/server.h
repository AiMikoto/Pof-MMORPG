#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include "chat_server/client.h"
#include <boost/thread/barrier.hpp>

class server
{
public:
  server(int port);
  ~server();
private:
  void routine();
  void cleanup();
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::barrier bar;
  std::vector<client*> clients;
  bool shutdown;
};

#endif // CHAT_SERVER_SERVER_H

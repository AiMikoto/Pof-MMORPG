#ifndef CHAT_SERVER_SERVER_H
#define CHAT_SERVER_SERVER_H

#include <boost/asio.hpp>
#include <vector>
#include "chat_server/client.h"

class server
{
public:
  server(int port);
  ~server();
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  std::vector<client*> clients;
  bool shutdown;
  boost::thread *t_routine;
  boost::thread *t_cleanup;
};

void routine(server *that);

void cleanup(server *that);

#endif // CHAT_SERVER_SERVER_H

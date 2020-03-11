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
  boost::asio::ip::tcp::endpoint endpoint;
  boost::asio::ip::tcp::acceptor acceptor;
  boost::barrier acceptor_barrier;
  std::vector<client*> clients;
  bool shutdown;
  boost::thread *t_routine;
  boost::thread *t_cleanup;
};

void routine(server *that);

void cleanup(server *that);

#endif // CHAT_SERVER_SERVER_H

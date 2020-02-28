#ifndef LIB_PROTOCOL_H
#define LIB_PROTOCOL_H

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "lib/call.h"

#define OPCODE "opcode"
#define OP_PING "ping"
#define OP_PONG "pong"
#define OP_AUTH "auth"
#define OP_TERMINATE "term"

class protocol
{
public:
  protocol(boost::asio::ip::tcp::socket *sock);
  ~protocol();
  void close();
protected:
  void handle_ping(call c);
  void handle_pong(call c);
  void terminate(call c);
  void terminate_force(call c);
  boost::asio::ip::tcp::socket *socket;
  endpoint_table ept;
private:
  void routine();
  boost::thread *t_routine;
};

#endif // LIB_PROTOCOL_H

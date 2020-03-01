#ifndef LIB_PROTOCOL_H
#define LIB_PROTOCOL_H

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include "lib/call.h"
#include <map>

#define OPCODE "opcode"
#define OP_PING "ping"
#define OP_PONG "pong"
#define OP_AUTH "auth"
#define OP_AUTH_TOKEN "auth_tok"
#define OP_MOVE "move"
#define OP_CMD "command"
#define OP_UC_TRANS_ALL "ucta"
#define OP_UC_TRANS "uct"
#define OP_TERMINATE "term"
#define OP_REQUEST_CHANGE_MAP "req_change_map"

class protocol
{
public:
  protocol(boost::asio::ip::tcp::socket *sock);
  protocol(boost::asio::ip::tcp::socket *sock, int ping_freq);
  ~protocol();
  int safe_write(call c);
  void close();
  int get_ping();
protected:
  void handle_ping(call c);
  void handle_pong(call c);
  void terminate(call c);
  void terminate_force(call c);
  boost::asio::ip::tcp::socket *socket;
  endpoint_table ept;
private:
  int ping;
  void routine();
  void latency_service(int ping_freq);
  boost::thread *t_routine;
  boost::thread *t_pinger;
  std::map<std::string, boost::chrono::high_resolution_clock::time_point> pings;
};

#endif // LIB_PROTOCOL_H

#ifndef LIB_PROTOCOL_H
#define LIB_PROTOCOL_H

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/chrono.hpp>
#include "lib/call.h"
#include "lib/crypto.h"
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
#define OP_REQUEST_CHANGE_MAP_CB "ans_change_map"
#define OP_IRC "irc"
#define OP_IRC_S "irc_sub"
#define OP_IRC_U "irc_unsub"
#define OP_INSTANCE_MANAGEMENT_DEACTIVATE "ins_de"
#define OP_INSTANCE_MANAGEMENT_REACTIVATE "ins_re"
#define OP_RCON_SHUTDOWN "rcon_shutdown"
#define OP_SHUTDOWN "shutdown"
#define OP_SLICE "slice"
#define OP_SCENE "scene"

#define OP_EDIT_CB "e_cb"
#define OP_EDIT_SLICER_STATUS "e_ss"
#define OP_EDIT_SPS "e_sps"
#define OP_EDIT_MOVE_OBJECT "e_mov"
#define OP_EDIT_ROTATE_OBJECT "e_rot"
#define OP_EDIT_SAVE "e_sav"

class protocol
{
public:
  protocol(boost::asio::ip::tcp::socket *sock, crypto *c);
  protocol(boost::asio::ip::tcp::socket *sock, crypto *c, int ping_freq);
  ~protocol();
  void start();
  void start_ping();
  void replace_crypto(crypto *cry);
  int safe_write(call c);
  void close();
  int get_ping();
// careful with ept
  endpoint_table ept;
protected:
  void handle_ping(call c);
  void handle_pong(call c);
  void terminate(call c);
  void terminate_force(call c);
  crypto *aes;
  boost::asio::ip::tcp::socket *socket;
private:
  bool aes_enabled;
  crypto *cry;
  int ping;
  int ping_freq;
  void routine();
  void latency_service();
  boost::thread *t_routine;
  boost::thread *t_pinger;
  std::map<std::string, boost::chrono::high_resolution_clock::time_point> pings;
  bool terminated;
};

#endif // LIB_PROTOCOL_H

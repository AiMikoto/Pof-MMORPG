#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include "lib/protocol.h"

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
  std::string username;
private:
  void validate_authority(std::string token);
  void uc_transfer(call c);
  void handle_auth(call c);
  void handle_map_change_request(call c);
  void handle_map_change_request_cb(call c);
  void handle_cmd(call c);
  void handle_irc_request(call c);
  void handle_shutdown(call c);
  // editor calls
  void set_slicer(call c);
};

extern client *master;

#endif // INSTANCE_CLIENT_H

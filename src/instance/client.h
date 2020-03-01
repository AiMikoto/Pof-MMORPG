#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include "lib/protocol.h"

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  void validate_authority(std::string token);
  void uc_transfer(call c);
  void handle_auth(call c);
  void handle_auth_helper(call c);
  void handle_map_change_request(call c);
  void handle_map_change_request_cb(call c);
  void handle_cmd(call c);
  std::string username;
};

#endif

#ifndef INSTANCE_CLIENT_H
#define INSTANCE_CLIENT_H

#include "lib/protocol.h"

void init_crypto(std::string priv);

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  crypto *aes;
  void validate_authority(std::string token);
  void uc_transfer(call c);
  void handle_auth(call c);
  void handle_auth_helper(call c);
  void handle_map_change_request(call c);
  void handle_map_change_request_cb(call c);
  void handle_cmd(call c);
  std::string username;
};

#endif // INSTANCE_CLIENT_H

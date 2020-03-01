#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include "lib/protocol.h"

class client:public protocol
{
public:
  client(boost::asio::ip::tcp::socket *sock);
  ~client();
private:
  void handle_auth(call c);
};

#endif // SERVER_CLIENT_H

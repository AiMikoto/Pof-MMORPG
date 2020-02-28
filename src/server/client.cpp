#include "server/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_AUTH, boost::bind(&client::handle_auth, this, _1));
}

client::~client()
{
}

void client::handle_auth(call c)
{
  std::string username = c.tree().get<std::string>("login.username");
  std::string password = c.tree().get<std::string>("login.password");
  call answer;
  answer.tree().put(OPCODE, OP_AUTH);
  if(true) // TODO: use username and password to confirm authentification
  {
    answer.tree().put("success", true);
    // TODO: assign login token to user
    // TODO: load user card
    write_call(socket, answer);
    // TODO: transfer user card to instance
    call term;
    term.tree().put(OPCODE, OP_TERMINATE);
//    write_call(socket, term);
  }
  else
  {
    answer.tree().put("success", false);
    write_call(socket, answer);
  }
}

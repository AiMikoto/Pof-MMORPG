#include "chat_server/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "lib/chat.h"
#include "chat_server/rooms.h"
#include "chat_server/crypto.h"
#include "chat_server/shutdown.h"
#include "chat_server/token.h"

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, 10)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_SHUTDOWN, boost::bind(&client::handle_shutdown, this, _1));
  ept.add(OP_CMD, boost::bind(&client::handle_cmd, this, _1));
  start();
}

client::~client()
{
}

// throws instead of returning.
void client::validate_authority(std::string auth_tok)
{
  if(auth_tok != my_token)
  {
    throw std::logic_error("wrong authority token");
  }
}

void client::handle_cmd(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  std::string command = c.tree().get<std::string>("command");
  if(command == "hello")
  { // uses RSA
    std::string key = c.tree().get<std::string>("aes.key");
    std::string iv = c.tree().get<std::string>("aes.iv");
    aes = new aes_crypto(key, iv);
    replace_crypto(aes);
    ept.add(OP_IRC, boost::bind(&client::handle_irc_request, this, _1));
    ept.add(OP_IRC_S, boost::bind(&client::handle_subscribe, this, _1));
    ept.add(OP_IRC_U, boost::bind(&client::handle_unsubscribe, this, _1));
    return;
  }
  if(command == "rcon")
  { // uses RSA
    BOOST_LOG_TRIVIAL(warning) << "received rcon authentication";
    std::string key = c.tree().get<std::string>("aes.key");
    std::string iv = c.tree().get<std::string>("aes.iv");
    aes = new aes_crypto(key, iv);
    replace_crypto(aes);
    return;
  }
  BOOST_LOG_TRIVIAL(warning) << "unknown command - " << command;
}

void client::handle_shutdown(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  BOOST_LOG_TRIVIAL(trace) << "remote shutdown initiated";
  shutdown();
}

void client::handle_irc_request(call c)
{
  std::string target_room = c.tree().get<std::string>("meta.target");
  c.tree().erase("meta");
  give_message(target_room, c);
}

void client::handle_subscribe(call c)
{
  std::string target_room = c.tree().get<std::string>("meta.target");
  sub(target_room, this);
}

void client::handle_unsubscribe(call c)
{
  std::string target_room = c.tree().get<std::string>("meta.target");
  unsub(target_room, this);
}

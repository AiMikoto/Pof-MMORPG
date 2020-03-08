#include "chat_server/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "lib/chat.h"

std::string my_tok = "lion"; // TODO: export this

rsa_crypto *g_rsa;

void init_crypto(std::string priv)
{
  BIO *keybio = BIO_new(BIO_s_file());
  BIO_read_filename(keybio, priv.c_str());
  g_rsa = new rsa_crypto(NULL, keybio);
}

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa, 10)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_CMD, boost::bind(&client::handle_cmd, this, _1));
  start();
}

client::~client()
{
}

// throws instead of returning.
void client::validate_authority(std::string auth_tok)
{
  if(auth_tok != my_tok)
  {
    throw std::logic_error("wrong authority token");
  }
}

void client::handle_cmd(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  std::string command = c.tree().get<std::string>("command");
  if(command == "init")
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
  BOOST_LOG_TRIVIAL(warning) << "unknown command - " << command;
}

void client::handle_irc_request(call c)
{
}

void client::handle_subscribe(call c)
{
}

void client::handle_unsubscribe(call c)
{
}

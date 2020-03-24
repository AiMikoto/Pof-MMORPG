#include "server/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include "common/user_card.h"
#include "server/instances.h"
#include "include/maps.h"
#include "include/regions.h"
#include "lib/uuid.h"
#include "server/ioc.h"
#include "server/crypto.h"
#include "server/shutdown.h"
#include "server/token.h"

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_SHUTDOWN, boost::bind(&client::handle_shutdown, this, _1));
  ept.add(OP_CMD, boost::bind(&client::handle_cmd, this, _1));
  ept.add(OP_AUTH, boost::bind(&client::handle_auth, this, _1));
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

void client::handle_auth(call c)
{ // uses RSA
  std::string username = c.tree().get<std::string>("login.username");
  std::string password = c.tree().get<std::string>("login.password");
  std::string key = c.tree().get<std::string>("aes.key");
  std::string iv = c.tree().get<std::string>("aes.iv");
  aes = new aes_crypto(key, iv);
  replace_crypto(aes);
  call answer;
  answer.tree().put(OPCODE, OP_AUTH);
  int status = false;
  user_card uc = db -> auth(username, password, &status);
  if(status)
  {
    answer.tree().put("status", true);
    safe_write(answer);
    std::string token = get_uuid();
    uc.tree().put("user.token", token);
    call uc_transfer;
    uc_transfer.tree().put(OPCODE, OP_UC_TRANS_ALL);
    uc_transfer.tree().put_child("data", uc.tree());
    safe_write(uc_transfer);
    instance_info *target_instance = get_pub_in(REG_EU, MAP_FLATLANDS);
    target_instance -> transfer_user_card(uc);
    call move;
    move.tree().put(OPCODE, OP_MOVE);
    move.tree().put("target.host", target_instance -> hostname);
    move.tree().put("target.port", target_instance -> port);
    safe_write(move);
  }
  else
  {
    answer.tree().put("status", false);
    safe_write(answer);
  }
  this -> close();
}

void client::handle_cmd(call c)
{
  validate_authority(c.tree().get<std::string>("authority.token"));
  std::string command = c.tree().get<std::string>("command");
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

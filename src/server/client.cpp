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
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "server/ioc.h"

boost::uuids::random_generator generator;

client::client(boost::asio::ip::tcp::socket *sock):protocol(sock, g_rsa)
{
  BOOST_LOG_TRIVIAL(info) << "received new connection from " << socket -> remote_endpoint().address().to_string();
  ept.add(OP_AUTH, boost::bind(&client::handle_auth, this, _1));
  start();
}

client::~client()
{
}

void client::handle_auth(call c)
{ // uses RSA
  std::string username = c.tree().get<std::string>("login.username");
  std::string password = c.tree().get<std::string>("login.password");
  std::string key = c.tree().get<std::string>("aes.key");
  std::string iv = c.tree().get<std::string>("aes.iv");
  BOOST_LOG_TRIVIAL(trace) << "creating aes object";
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
    std::string token = boost::lexical_cast<std::string>(generator());
    call uc_transfer;
    uc_transfer.tree().put(OPCODE, OP_UC_TRANS_ALL);
    uc_transfer.tree().put_child("data", uc.tree());
    safe_write(uc_transfer);
    instance_info *target_instance = pins[REG_EU][MAP_FLATLANDS];
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

#include "server/client.h"
#include "lib/log.h"
#include "lib/protocol.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include "include/common_macro.h"
#include <exception>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include "common/user_card.h"
#include "server/instances.h"

boost::uuids::random_generator generator;

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
    safe_write(answer);
    user_card uc;
    // TODO: load user card
    std::string token = boost::lexical_cast<std::string>(generator());
    uc.tree().put("user.name", username);
    uc.tree().put("user.token", token);
    call uc_transfer;
    uc_transfer.tree().put(OPCODE, OP_UC_TRANS_ALL);
    uc_transfer.tree().put_child("data", uc.tree());
    // TODO: transfer user card to instance
    safe_write(uc_transfer);
    instance_info *target_instance = pins[REG_EU]["flatlands"];
    uc_transfer.tree().put("authority.token", target_instance -> auth_tok);
    target_instance -> in -> safe_write(uc_transfer);
    call move;
    move.tree().put(OPCODE, OP_MOVE);
    move.tree().put("target.host", target_instance -> hostname);
    move.tree().put("target.port", target_instance -> port);
    safe_write(move);
    call term;
    term.tree().put(OPCODE, OP_TERMINATE);
    safe_write(term);
  }
  else
  {
    answer.tree().put("success", false);
    safe_write(answer);
  }
}

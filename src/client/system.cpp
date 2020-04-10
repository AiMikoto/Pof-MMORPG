#include "client/system.h"
#include "lib/log.h"

client_system_manager::client_system_manager(chat_log *cl)
{
  this -> cl = cl;
}

client_system_manager::~client_system_manager()
{
  // do not delete cl, cl is the responsibility of the caller
}

void client_system_manager::say(std::string msg)
{ 
  BOOST_LOG_TRIVIAL(info) << msg;
  cl -> add(message(ct_system, msg));
}

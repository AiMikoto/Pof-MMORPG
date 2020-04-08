#include "instance/misc.h"
#include "instance/client.h"
#include "lib/chat.h"
#include "lib/log.h"
#include "instance/ioc.h"
#include <cstdio>

void send_message(user_card& uc, call c)
{
  chat_target target = static_cast<chat_target>(c.tree().get<int>("payload.target"));
  BOOST_LOG_TRIVIAL(trace) << "considering whether to send message to user";
  bool approve = false;
  if(target == local)
  {
    approve = true;
  }
  if(target == world)
  {
    approve = true;
  }
  if(approve)
  {
    BOOST_LOG_TRIVIAL(trace) << "message approved";
    client *cli = (client *)uc.aux;
    if(cli)
    {
      cli -> safe_write(c);
    }
  }
}

void saver(user_card &uc)
{
  db -> uc_save(uc.tree().get<std::string>("user.name"), uc);
}

void ucl_broadcast(user_card &uc, call c)
{
  client *cli = (client *)uc.aux;
  if(cli)
  {
    cli -> safe_write(c);
  }
}

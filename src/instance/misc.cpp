#include "instance/misc.h"
#include "instance/client.h"
#include "lib/chat.h"

void send_message(user_card& uc, call c)
{
  chat_target target = static_cast<chat_target>(c.tree().get<int>("target"));
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
    client *cli = (client *)uc.aux;
    cli -> safe_write(c);
  }
}


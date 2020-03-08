#include "instance/misc.h"
#include "instance/client.h"

void send_message(user_card& uc, call c)
{
  client *cli = (client *)uc.aux;
  cli -> safe_write(c);
}


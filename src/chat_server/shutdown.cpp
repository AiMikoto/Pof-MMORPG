#include "chat_server/shutdown.h"

boost::barrier main_barrier(2);

void shutdown()
{
  main_barrier.wait();
}

void shutdown(int _whatever)
{
  shutdown();
}

#include "client/shutdown.h"
#include <mutex>

boost::barrier main_barrier(2);
std::mutex shut_lock;
bool flag = true;

void shutdown()
{
  shut_lock.lock();
  if(flag)
  {
    flag = false;
    main_barrier.wait();
  }
  shut_lock.unlock();
}

void shutdown(int _whatever)
{
  shutdown();
}

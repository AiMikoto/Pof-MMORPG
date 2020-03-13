#ifndef INSTANCE_SHUTDOWN_H
#define INSTANCE_SHUTDOWN_H

#include <boost/thread/barrier.hpp>

void shutdown();

void shutdown(int _whatever);

extern boost::barrier main_barrier;

#endif // INSTANCE_SHUTDOWN_H

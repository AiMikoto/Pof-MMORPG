#ifndef INSTANCE_MISC_H
#define INSTANCE_MISC_H

#include "common/user_card.h"
#include "lib/call.h"
#include <boost/property_tree/ptree.hpp>

void send_message(user_card& uc, call c);

void saver(user_card& uc);

void ucl_broadcast(user_card &uc, call c);

#endif // INSTANCE_MISC_H

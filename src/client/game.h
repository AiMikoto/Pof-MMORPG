#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "common/user_card.h"
#include <mutex>

extern user_card_library ucl;

// extern game *g;

extern std::mutex init_l;

extern std::string username;

void move(std::string host, int port);

#endif // CLIENT_GAME_H

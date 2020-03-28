#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "common/user_card.h"
#include "lib/chat.h"
#include <mutex>
#include "phys/slicing.h"

extern user_card_library ucl;

extern engine::Scene *current;

extern std::mutex scene_lock;

extern chat_log cl;

extern std::mutex init_l;

extern std::string username;

void move(std::string host, int port);

void add_slice(slice_t next_slice);

void send_message(chat_target target, std::string payload);

#endif // CLIENT_GAME_H

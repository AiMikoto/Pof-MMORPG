#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "common/user_card.h"
#include "lib/chat.h"
#include <mutex>
#include "phys/slicing.h"
#include "scene/scene.h"
#include "client/system.h"

extern engine::Scene *current;

extern std::mutex scene_lock;

extern std::mutex init_l;

extern std::string username;

void game_init();

void game_destroy();

void move(std::string host, int port);

void set_scene(boost::property_tree::ptree node);

void add_slice(slice_t next_slice);

void wipe(std::string tag);

void apply_slice_buffer();

void send_message(chat_target target, std::string payload);

#endif // CLIENT_GAME_H

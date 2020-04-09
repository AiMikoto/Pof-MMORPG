#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include "common/user_card.h"
#include "lib/chat.h"
#include <mutex>
#include "phys/slicing.h"
#include "scene/scene.h"

extern user_card_library ucl;

extern engine::Scene *current;

extern std::mutex scene_lock;

extern chat_log cl;

extern std::mutex init_l;

extern std::string username;

void handle_linear_input(std::string s);

void move(std::string host, int port);

void set_scene(boost::property_tree::ptree node);

void add_slice(slice_t next_slice);

void wipe(std::string tag);

void apply_slice_buffer();

void send_message(chat_target target, std::string payload);

#endif // CLIENT_GAME_H

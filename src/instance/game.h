#ifndef INSTANCE_GAME_H
#define INSTANCE_GAME_H

#include "common/user_card.h"
#include "scene/scene.h"
#include "phys/slicing.h"
#include "include/maps.h"

extern user_card_library ucl;
extern user_card_library uclp;

void ucl_destroy();

extern engine::Scene *current;

void unload();

void save();

void save(map_t map);

void load(map_t map);

bool is_loaded();

unsigned long long game_inject_object();

void game_delete_object(unsigned long long id);

void slicer();

#endif // INSTANCE_GAME_H

#ifndef INSTANCE_GAME_H
#define INSTANCE_GAME_H

#include "common/user_card.h"
#include "scene/scene.h"
#include "phys/slicing.h"

extern user_card_library ucl;
extern user_card_library uclp;

void ucl_destroy();

extern engine::Scene *current;

void unload();

void load();

bool is_loaded();

void slicer();

#endif // INSTANCE_GAME_H

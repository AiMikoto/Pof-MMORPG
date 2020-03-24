#ifndef INSTANCE_GAME_H
#define INSTANCE_GAME_H

#include "common/user_card.h"

extern user_card_library ucl;
extern user_card_library uclp;

void ucl_destroy();

void unload();

void load();

bool is_loaded();

#endif // INSTANCE_GAME_H

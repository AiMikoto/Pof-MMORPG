#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include "scene/scene.h"

void gfx_init();

void gfx_push(engine::Scene *s);

void gfx_destroy();

void gfx_duty();

#endif // CLIENT_GRAPHICS_H

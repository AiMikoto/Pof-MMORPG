#ifndef CLIENT_GRAPHICS_H
#define CLIENT_GRAPHICS_H

#include "scene/scene.h"

void gfx_init();

void gfx_buffer(boost::property_tree::ptree);

void gfx_push();

void gfx_destroy();

void gfx_duty();

#endif // CLIENT_GRAPHICS_H

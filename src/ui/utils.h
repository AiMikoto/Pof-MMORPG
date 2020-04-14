#ifndef UI_UTILS_H
#define UI_UTILS_H

#include "ui/ui.h"
#include "scene/scene.h"
#include <glm/glm.hpp>
#include <string>
#include "scene/oid.h"
#include <boost/function.hpp>

typedef boost::function<void(std::string)> UI_linear_callback;

#define H_GET(x) x.c_str(), x.size(), 1

void draw_game_object(ctx_t *ctx, engine::GameObject *o, std::string path, oid_t& oid);

void draw_component(ctx_t *ctx, engine::Component *c, std::string path);

void draw_transform(ctx_t *ctx, engine::Transform t, std::string path);

void draw_dvec(ctx_t *ctx, glm::dvec3 v);

void draw_dvec(ctx_t *ctx, glm::dvec4 v);

void draw_dvec(ctx_t *ctx, glm::dquat v);

#endif // UI_UTILS_H

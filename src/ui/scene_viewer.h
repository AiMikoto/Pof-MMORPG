#ifndef UI_SCENE_VIEWER_H
#define UI_SCENE_VIEWER_H

#include "ui/ui.h"
#include "scene/scene.h"
#include <glm/glm.hpp>

class UI_scene_viewer : public UI_element
{
public:
  UI_scene_viewer(engine::Scene **s);
  void init(ctx_t *ctx);
  void visit(ctx_t *ctx);
private:
  void draw(ctx_t *ctx);
  void draw_game_object(ctx_t *ctx, engine::GameObject *o);
  void draw_component(ctx_t *ctx, engine::Component *c);
  void draw_transform(ctx_t *ctx, engine::Transform t);
  void draw_dvec(ctx_t *ctx, glm::dvec3 v);
  void draw_dvec(ctx_t *ctx, glm::dvec4 v);
  void draw_dvec(ctx_t *ctx, glm::dquat v);
  engine::Scene **s;
};

#endif // UI_SCENE_VIEWER_H

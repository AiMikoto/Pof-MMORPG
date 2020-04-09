#include "ui/scene_viewer.h"

UI_scene_viewer::UI_scene_viewer(engine::Scene **s)
{
  this -> s = s;
}

void UI_scene_viewer::init(ctx_t *ctx)
{
}

void UI_scene_viewer::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_scene_viewer::draw(ctx_t *ctx)
{
  if(nk_begin(ctx, "SceneViewer", nk_rect(20, 30, 150, 600), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE))
  {
    engine::Scene *scene = *s;
    if(scene == NULL)
    {
      goto UI_scene_viewer_draw_end;
    }
    for(auto it : scene -> gameObjects)
    {
      draw_game_object(ctx, it.second);
    }
  }
  UI_scene_viewer_draw_end:
  nk_end(ctx);
}

void UI_scene_viewer::draw_game_object(ctx_t *ctx, engine::GameObject *o)
{
  for(auto c : o -> components)
  {
    draw_component(ctx, c);
  }
}

void UI_scene_viewer::draw_component(ctx_t *ctx, engine::Component *c)
{
}

#include "ui/scene_viewer.h"
#include "components/meshFilter.h"
#include "components/meshRenderer.h"
#include "components/solid_object.h"
#include "components/phys_collider.h"
#include "lib/uuid.h"
#include "ui/utils.h"
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>

#define H_GET(x) x.c_str(), x.size(), 1

bool match(std::string str, std::string substr)
{ // match is true if substr is in str
  return boost::algorithm::knuth_morris_pratt_search(str.begin(), str.end(), substr.begin(), substr.end()).first != str.end();
}

UI_object_viewer::UI_object_viewer(engine::Scene **s, oid_t oid)
{
  this -> s = s;
  this -> uuid = get_uuid();
  this -> oid = oid;
}

void UI_object_viewer::init(ctx_t *ctx)
{
}

void UI_object_viewer::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_object_viewer::destroy(ctx_t *ctx)
{
  if(suicide)
  {
    nk_window_close(ctx, uuid.c_str());
  }
}

void UI_object_viewer::draw(ctx_t *ctx)
{ 
  std::string path = uuid;
  if(nk_begin_titled(ctx, path.c_str(), "Object Viewer", nk_rect(20, 30, 200, 300), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_CLOSABLE))
  {
    engine::Scene *scene = *s;
    if(scene == NULL)
    {
      suicide = true;
      goto UI_object_viewer_draw_end;
    }
    engine::GameObject *object = oid.get(scene);
    if(object == NULL)
    {
      suicide = true;
      goto UI_object_viewer_draw_end;
    }
    std::string opath = path + "o";
    draw_game_object(ctx, object, opath, oid);
  }
  suicide = suicide | nk_window_is_hidden(ctx, path.c_str());
  UI_object_viewer_draw_end:
  nk_end(ctx);
}

UI_scene_viewer::UI_scene_viewer(engine::Scene **s)
{
  this -> s = s;
  this -> uuid = get_uuid();
  this -> filter = "";
}

void UI_scene_viewer::init(ctx_t *ctx)
{
}

void UI_scene_viewer::visit(ctx_t *ctx)
{
  draw(ctx);
}

void UI_scene_viewer::destroy(ctx_t *ctx)
{
  if(suicide)
  {
    nk_window_close(ctx, uuid.c_str());
  }
}

void UI_scene_viewer::draw(ctx_t *ctx)
{
  oid_t oid;
  std::string path = uuid;
  if(nk_begin_titled(ctx, path.c_str(), "Scene Viewer", nk_rect(20, 30, 150, 600), NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE))
  {
    nk_layout_row_dynamic(ctx, 23, 2);
    nk_flags event = nk_edit_string(ctx, NK_EDIT_FIELD | NK_EDIT_SIG_ENTER, buf, &len, UI_SCENE_VIEWER_BUF_SIZE - 1, nk_filter_default);
    if(event & NK_EDIT_COMMITED)
    {
      filter = std::string(buf, len);
    }
    if(nk_button_label(ctx, "Reset filter"))
    {
      filter == "";
    }
    engine::Scene *scene = *s;
    if(scene == NULL)
    {
      goto UI_scene_viewer_draw_end;
    }
    std::string opath = path + "o";
    if(nk_tree_push_hashed(ctx, NK_TREE_TAB, "Objects", NK_MINIMIZED, H_GET(opath)))
    {
      for(auto it : scene -> children)
      {
        oid.at(it.first);
        bool matches = false;
        matches = matches || match(it.second -> name, filter);
        for(auto tag : it.second -> tag)
        {
          matches = matches || match(tag, filter);
        }
        if(matches)
        {
          std::string oopath = opath + std::to_string(it.first);
          draw_game_object(ctx, it.second, oopath, oid);
        }
        oid.pop();
      }
      nk_tree_pop(ctx);
    }
  }
  UI_scene_viewer_draw_end:
  nk_end(ctx);
}

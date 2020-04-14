#include "phys/slicing.h"
#include "lib/log.h"
#include "phys/collisions.h"
#include "components/solid_object.h"
#include "components/phys_collider.h"
#include <set>
#include <mutex>

std::mutex slicer_lock;

// a few helper functions

collision_handling_model_t collision_handling_model = CH_SM;

glm::dvec3 decode_dvec3(boost::property_tree::ptree tree)
{
  return {tree.get<double>("x"), tree.get<double>("y"), tree.get<double>("z")};
}

boost::property_tree::ptree encode_dvec3(glm::dvec3 vector)
{
  boost::property_tree::ptree ret;
  ret.put("x", vector.x);
  ret.put("y", vector.y);
  ret.put("z", vector.z);
  return ret;
}

// slice_t implementation

slice_t::slice_t()
{
  this -> origin_generation = 0;
  this -> target_generation = 0;
  this -> tag = "";
}

slice_t::slice_t(boost::property_tree::ptree tree)
{
  boost::property_tree::ptree pos_node = tree.get_child("pos");
  boost::property_tree::ptree vel_node = tree.get_child("vel");
  boost::property_tree::ptree shift_node = tree.get_child("shift");
  boost::property_tree::ptree scale_node = tree.get_child("scale");
  boost::property_tree::ptree rotation_node = tree.get_child("rotation");
  boost::property_tree::ptree object_node = tree.get_child("obj");
  boost::property_tree::ptree component_node = tree.get_child("comp");
  boost::property_tree::ptree eject_node = tree.get_child("eje");
  boost::property_tree::ptree renames_node = tree.get_child("renames");
  boost::property_tree::ptree tags_node = tree.get_child("tags");
  boost::property_tree::ptree untags_node = tree.get_child("untags");
  for(auto it : pos_node)
  {
    this -> pos_delta[oid_t(it.first)] = decode_dvec3(it.second);
  }
  for(auto it : vel_node)
  {
    this -> vel_delta[oid_t(it.first)] = decode_dvec3(it.second);
  }
  for(auto it : shift_node)
  {
    this -> shift[oid_t(it.first)] = decode_dvec3(it.second);
  }
  for(auto it : scale_node)
  {
    this -> scale[oid_t(it.first)] = decode_dvec3(it.second);
  }
  for(auto it : rotation_node)
  {
    this -> rotation[oid_t(it.first)] = decode_dvec3(it.second);
  }
  for(auto it : object_node)
  {
    this -> objects[oid_t(it.first)] = it.second;
  }
  for(auto it : component_node)
  {
    this -> components[oid_t(it.first)] = it.second;
  }
  for(auto it : eject_node)
  {
    this -> ejections.push_back(oid_t(it.first));
  }
  for(auto it : renames_node)
  {
    this -> renames[oid_t(it.first)] = it.second.get<std::string>("");
  }
  for(auto it : tags_node)
  {
    this -> tags[oid_t(it.first)] = it.second.get<std::string>("");
  }
  for(auto it : untags_node)
  {
    this -> untags[oid_t(it.first)] = it.second.get<std::string>("");
  }
  this -> origin_generation = tree.get<long long>("og");
  this -> target_generation = tree.get<long long>("tg");
  this -> tag = tree.get<std::string>("tag");
}

boost::property_tree::ptree slice_t::encode()
{
  boost::property_tree::ptree ret, pos_node, vel_node, shift_node, scale_node, rotation_node, object_node, component_node, eject_node, renames_node, tags_node, untags_node;
  for(auto it : this -> pos_delta)
  {
    pos_node.put_child(it.first.serialise('+'), encode_dvec3(it.second));
  }
  for(auto it : this -> vel_delta)
  {
    vel_node.put_child(it.first.serialise('+'), encode_dvec3(it.second));
  }
  for(auto it : this -> shift)
  {
    shift_node.put_child(it.first.serialise('+'), encode_dvec3(it.second));
  }
  for(auto it : this -> scale)
  {
    scale_node.put_child(it.first.serialise('+'), encode_dvec3(it.second));
  }
  for(auto it : this -> rotation)
  {
    rotation_node.put_child(it.first.serialise('+'), encode_dvec3(it.second));
  }
  for(auto it : this -> objects)
  {
    object_node.put_child(it.first.serialise('+'), it.second);
  }
  for(auto it : this -> components)
  {
    component_node.put_child(it.first.serialise('+'), it.second);
  }
  for(auto it : this -> ejections)
  {
    eject_node.put(it.serialise('+'), it.serialise('+'));
  }
  for(auto it : this -> renames)
  {
    renames_node.put(it.first.serialise('+'), it.second);
  }
  for(auto it : this -> tags)
  {
    tags_node.put(it.first.serialise('+'), it.second);
  }
  for(auto it : this -> untags)
  {
    untags_node.put(it.first.serialise('+'), it.second);
  }
  ret.put_child("pos", pos_node);
  ret.put_child("vel", vel_node);
  ret.put_child("shift", shift_node);
  ret.put_child("scale", scale_node);
  ret.put_child("rotation", rotation_node);
  ret.put_child("obj", object_node);
  ret.put_child("comp", component_node);
  ret.put_child("eje", eject_node);
  ret.put_child("renames", renames_node);
  ret.put_child("tags", tags_node);
  ret.put_child("untags", untags_node);
  ret.put("og", this -> origin_generation);
  ret.put("tg", this -> target_generation);
  ret.put("tag", this -> tag);
  return ret;
}

// slicing interference

glm::dvec3 gravity_vector = {0, -2, 0};
std::map <oid_t, glm::dvec3> slicer_injection_shift;
std::map <oid_t, glm::dvec3> slicer_injection_scale;
std::map <oid_t, glm::dvec3> slicer_injection_rotation;
std::map <oid_t, boost::property_tree::ptree> slicer_injection_objects;
std::vector <oid_t> slicer_ejection_objects;
std::map <oid_t, boost::property_tree::ptree> slicer_injection_components;
std::map <oid_t, std::string> slicer_inject_name;
std::map <oid_t, std::string> slicer_inject_tag;
std::map <oid_t, std::string> slicer_eject_tag;

// slicing constants

bool slicer_active = true;
double SPS = 100; // Slices per second
double BAUMGARDE_CONSTANT = 0.2;
double COEFFICIENT_OF_RESTITUTION = 0.3;

// slicing implementation

slice_t slice(engine::Scene *e)
{
  slice_t ret;
  ret.origin_generation = e -> generation;
  ret.target_generation = ret.origin_generation + 1;
  ret.tag = e -> tag;
  double dt = 1 / SPS;
  slicer_lock.lock();
  ret.shift = slicer_injection_shift;
  slicer_injection_shift.clear();
  ret.scale = slicer_injection_scale;
  slicer_injection_scale.clear();
  ret.rotation = slicer_injection_rotation;
  slicer_injection_rotation.clear();
  ret.objects = slicer_injection_objects;
  slicer_injection_objects.clear();
  ret.components = slicer_injection_components;
  slicer_injection_components.clear();
  ret.ejections = slicer_ejection_objects;
  slicer_ejection_objects.clear();
  ret.renames = slicer_inject_name;
  slicer_inject_name.clear();
  ret.tags = slicer_inject_tag;
  slicer_inject_tag.clear();
  ret.untags = slicer_eject_tag;
  slicer_eject_tag.clear();
  if(!slicer_active)
  {
    slicer_lock.unlock();
    return ret;
  }
  for(auto it : e -> children)
  {
    oid_t oid;
    oid.at(it.first);
    engine::GameObject *go = it.second;
    solid_object *gop = go -> getComponent<solid_object>();
    if(!gop)
    {
      continue;
    }
    else
    {
      // adding gravity vector
      gop -> force_acc += gravity_vector;
      ret.vel_delta[oid] = gop -> im * gop -> force_acc * dt;
      gop -> force_acc = {0, 0, 0};
      ret.pos_delta[oid] = (gop -> velocity + ret.vel_delta[oid]) * dt;
      collider *c = go -> getComponent<physical_collider>();
      if(!c)
      { // pointless checking for collisions without a collider
        continue;
      }
      // emulating position change for the purpose of collision detection
      go -> transform.position += ret.pos_delta[oid];
      aabb caabb = c -> to_aabb();
      BOOST_LOG_TRIVIAL(trace) << "Checking for colisions";
      std::set<unsigned long long> collisions;
      collisions = e -> ctree.get_collisions(caabb);
      collisions.erase(it.first);
      BOOST_LOG_TRIVIAL(trace) << "Handling collisions";
      for(auto collision:collisions)
      {
        glm::dvec3 axis;
        double offset;
        physical_collider *phys = e -> children[collision] -> getComponent<physical_collider>();
        if(!phys)
        {
          continue;
        }
        if(collide(phys, c, &axis, &offset))
        {
          if(offset < 0.000001)
          {
            continue;
          }
          BOOST_LOG_TRIVIAL(trace) << "handling collision";
          switch (collision_handling_model)
          {
            case CH_SI:
            {
              double bias = -offset * BAUMGARDE_CONSTANT / dt + COEFFICIENT_OF_RESTITUTION * glm::dot(-(gop -> velocity + ret.vel_delta[oid]), axis);
              glm::dvec3 j = axis;
              glm::dmat3 im = {{gop -> im, 0, 0}, {0, gop -> im, 0}, {0, 0, gop -> im}};
              glm::dvec3 v = (gop -> velocity + ret.vel_delta[oid]);
              double em = glm::dot(j, im * j);
              double lam = (-glm::dot(j, v) + bias) / em;
              glm::dvec3 dv = (im * j) * lam;
              ret.vel_delta[oid] += dv;
              break;
            }
            case CH_SM:
            {
              // stop the object
              ret.vel_delta[oid] -= glm::dot(gop -> velocity + ret.vel_delta[oid], axis) * axis;
              // push the object back
              go -> transform.position -= ret.pos_delta[oid];
              ret.pos_delta[oid] -= (axis * offset);
              go -> transform.position += ret.pos_delta[oid];
              break;
            }
            default:
            {
              // literally nothing, it will clip through
              break;
            }
          }
        }
        else
        {
          BOOST_LOG_TRIVIAL(trace) << "false collision";
        }
      }
      // undo position change emulation
      go -> transform.position -= ret.pos_delta[oid];
    }
  }
  slicer_lock.unlock();
  return ret;
}

engine::Scene *apply_slice(engine::Scene *e, slice_t slice)
{
  e -> generation = slice.target_generation;
  for(auto it : slice.pos_delta)
  {
    engine::GameObject *go = it.first.get(e);
    go -> transform.position += it.second;
  }
  for(auto it : slice.vel_delta)
  {
    engine::GameObject *go = it.first.get(e);
    solid_object *gop = go -> getComponent<solid_object>();
    gop -> velocity += it.second;
  }
  for(auto it : slice.shift)
  {
    engine::GameObject *go = it.first.get(e);
    go -> transform.position = it.second;
  }
  for(auto it : slice.scale)
  {
    engine::GameObject *go = it.first.get(e);
    go -> transform.scale = it.second;
  }
  for(auto it : slice.rotation)
  {
    engine::GameObject *go = it.first.get(e);
    go -> transform.rotateTo(it.second);
  }
  for(auto it : slice.ejections)
  {
    it.destroy(e);
  }
  for(auto it : slice.objects)
  {
    engine::GameObject *go = new engine::GameObject(it.second);
    it.first.put(e, go);
  }
  for(auto it : slice.components)
  {
    engine::GameObject *go = it.first.get(e);
    go -> constructComponent(it.second);
  }
  for(auto it : slice.renames)
  {
    engine::GameObject *go = it.first.get(e);
    go -> name = it.second;
  }
  for(auto it : slice.tags)
  {
    engine::GameObject *go = it.first.get(e);
    go -> tag.insert(it.second);
  }
  for(auto it : slice.untags)
  {
    engine::GameObject *go = it.first.get(e);
    go -> tag.erase(it.second);
  }
  e -> regenerateCtree();
  return e;
}

engine::Scene *tick(engine::Scene *e)
{
  return apply_slice(e, slice(e));
}

// editor functions

void slicer_set_status(bool status)
{
  slicer_active = status;
}

void slicer_set_sps(double val)
{
  SPS = val;
}

void slicer_move(oid_t &id, glm::dvec3 pos)
{
  slicer_injection_shift[id] = pos;
}

void slicer_scale(oid_t &id, glm::dvec3 scale)
{
  slicer_injection_scale[id] = scale;
}

void slicer_rotate(oid_t &id, glm::dvec3 rotation)
{
  slicer_injection_rotation[id] = rotation;
}

void slicer_inject_object(oid_t &id, engine::GameObject *go)
{
  slicer_injection_objects[id] = go -> serialize();
}

void slicer_inject_component(oid_t &id, engine::Component *c)
{
  boost::property_tree::ptree node;
  node.put_child(c -> name, c -> serialize());
  slicer_injection_components[id] = node;
}

void slicer_eject_object(oid_t &id)
{
  slicer_ejection_objects.push_back(id);
}

void slicer_rename_object(oid_t &id, std::string name)
{
  slicer_inject_name[id] = name;
}

void slicer_add_tag_object(oid_t &id, std::string tag)
{
  slicer_inject_tag[id] = tag;
}

void slicer_remove_tag_object(oid_t &id, std::string tag)
{
  slicer_eject_tag[id] = tag;
}

// lock functions - use these to control the slicing process, the functions above are only thread safe in the context of slicer_lock manipulation

void slicer_acquire()
{
  slicer_lock.lock();
}

void slicer_release()
{
  slicer_lock.unlock();
}

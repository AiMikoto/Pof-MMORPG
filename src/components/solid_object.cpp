#include "components/solid_object.h"
#include "core/utils.h"

solid_object::solid_object() : Component(false)
{
  velocity = {0, 0, 0};
  force_acc = {0, 0, 0};
  m = 1;
  im = 1;
  setType();
}

solid_object::solid_object(boost::property_tree::ptree node) : Component(false)
{
  force_acc = engine::vecDeserializer<glm::dvec3, double>(node.get_child("force_acc"));
  velocity = engine::vecDeserializer<glm::dvec3, double>(node.get_child("velocity"));
  m = node.get<double>("m");
  im = node.get<double>("im");
  setType();
}

solid_object::~solid_object()
{
}

void solid_object::setType()
{
  type = typeid(*this).name();
  name = "solid_object";
}

void solid_object::add_force(glm::dvec3 force)
{
  this -> force_acc += force;
}

boost::property_tree::ptree solid_object::serialize()
{
  boost::property_tree::ptree tree;
  tree.put("m", m);
  tree.put("im", im);
  tree.put_child("velocity", engine::vecSerializer(velocity));
  tree.put_child("force_acc", engine::vecSerializer(force_acc));
  return tree;
}

void solid_object::setup()
{
}

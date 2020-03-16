#pragma once
#include <vector>
#include <string>
#include "transform.h"
#include <boost/property_tree/ptree.hpp>

namespace graphics {
	class Component;

	class GameObject {
	public:
		std::string name, tag;
		GameObject* parent;
		std::vector<GameObject*> children;
		std::vector<Component*> components;
		Transform transform;
		uint type;

		GameObject();
		~GameObject();
		GameObject(GameObject* parent);
		GameObject(std::vector<GameObject*> children);
		GameObject(std::vector<Component*> components);
		GameObject(GameObject* parent, std::vector<GameObject*> children);
		GameObject(GameObject* parent, std::vector<Component*> components);
		GameObject(std::vector<GameObject*> children, std::vector<Component*> components);
		GameObject(GameObject* parent, std::vector<GameObject*> children, std::vector<Component*> components);

		//returns a pointer to a new game object that has all its values copied from the current one
		GameObject* instantiate();
		void addChild(GameObject* child);
		void addChildren(std::vector<GameObject*> children);
		void addComponent(Component* component);
		void addComponents(std::vector<Component*> components);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);	
	};
}

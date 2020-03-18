#pragma once
#include <vector>
#include <string>
#include "transform.h"
#include <boost/property_tree/ptree.hpp>
#include <typeinfo>
#include <algorithm>
#include "lib/log.h"
#include "utils.h"

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
		bool isStatic;

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
		virtual void update();
		void addChild(GameObject* child);
		void addChildren(std::vector<GameObject*> children);
		void addComponent(Component* component);
		void addComponents(std::vector<Component*> components);
		boost::property_tree::ptree serialize();
		void deserialize(boost::property_tree::ptree node);
		template<typename T> bool hasComponent() {
			std::string type = typeidToClassName(typeid(T).name());
			for (auto c : components) {
				if (type == c->type)
					return true;
			}
			return false;
		}
		template<typename T> T* getComponent() {
			std::string type = typeidToClassName(typeid(T).name());
			for (auto c : components) {
				if (type == c->type) {
					return static_cast<T*>(c);
				}
			}
			return NULL;
		}
		template<typename T> std::vector<T*> getComponents() {
			std::vector<T*> components;
			std::string type = typeidToClassName(typeid(T).name());
			for (auto c : this->components) {
				if (type == c->type)
					components.push_back(static_cast<T*>(c));
			}
			return components;
		}
		//a component can't exist outside a game object, so this will only receive new references
		template<typename T> bool addComponent(T* component) {
			if (!component->allowMultiple && hasComponent<T>())
				return false;
			this->components.push_back(component);
			component->gameObject = this;
			component->setup();
			return true;
		}
		//only ever called from the components destructor, don't call it separately, as a component can't exist outside of a game object
		template<typename T> void removeComponent(const T* component) {
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
		}
	};
}

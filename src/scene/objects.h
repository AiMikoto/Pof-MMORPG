#pragma once
#include <vector>
#include <string>
#include "scene/transform.h"
#include <boost/property_tree/ptree.hpp>
#include <typeinfo>
#include <algorithm>
#include "core/utils.h"
#include "core/constants.h"
#include "components/component.h"

namespace engine {

	class GameObject {
	public:
		std::string name, tag;
		GameObject* parent;
		std::vector<GameObject*> children;
		std::vector<Component*> components;
		Transform transform;
		bool isStatic;
		ullong id = 0;

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
			for (auto c : components) {
				if (typeid(T).name() == c->type)
					return true;
			}
			return false;
		}
		template<typename T> T* getComponent() {
			for (auto c : components) {
				if (typeid(T).name() == c->type) {
					return static_cast<T*>(c);
				}
			}
			return NULL;
		}
		template<typename T> T* getComponentInParents() {
			if (parent != NULL) {
				for (auto c : parent->components) {
					if (typeid(T).name() == c->type) {
						return static_cast<T*>(c);
					}
				}
				parent->getComponentInParents<T>();
			}
			return NULL;
		}
		template<typename T> T* getComponentInChildren() {
			for(auto child : children) {
				for (auto c : child->components) {
					if (typeid(T).name() == c->type) {
						return static_cast<T*>(c);
					}
				}
				child->getComponentInChildren<T>();
			}
			return NULL;
		}
		template<typename T> std::vector<T*> getComponents() {
			std::vector<T*> components;
			for (auto c : this->components) {
				if (typeid(T).name() == c->type)
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
		//don't call a component destructor separate from this function
		template<typename T> void removeComponent(T* component) {
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
			delete component;
		}
	};
}

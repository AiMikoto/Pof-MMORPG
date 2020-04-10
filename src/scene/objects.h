#pragma once
#include <map>
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
		std::map<ullong, GameObject*> children;
		std::map<ullong, Component*> components;
		Transform transform;
		bool isStatic;

		GameObject();
		~GameObject();
		GameObject(const GameObject& gameObject);
		GameObject(boost::property_tree::ptree node);
		GameObject(GameObject* parent);
		GameObject(std::map<ullong, GameObject*> children);
		GameObject(std::map<ullong, Component*> components);
		GameObject(GameObject* parent, std::map<ullong, GameObject*> children);
		GameObject(GameObject* parent, std::map<ullong, Component*> components);
		GameObject(std::map<ullong, GameObject*> children, std::map<ullong, Component*> components);
		GameObject(GameObject* parent, std::map<ullong, GameObject*> children, std::map<ullong, Component*> components);
		GameObject at(ullong index);
		virtual void update();
		void addChild(GameObject* child);
		void addChild(ullong id, GameObject* child);
		void addChildren(std::map<ullong, GameObject*> children);
		boost::property_tree::ptree serialize();
		template<typename T> bool hasComponent() {
			for (auto it : this->components) {
				engine::Component *c = it.second;
				if (typeid(T).name() == c->type)
					return true;
			}
			return false;
		}
		template<typename T> T* getComponent() {
			for (auto it : this->components) {
				engine::Component *c = it.second;
				if (typeid(T).name() == c->type) {
					return static_cast<T*>(c);
				}
			}
			return NULL;
		}
		template<typename T> T* getComponentInParents() {
			if (parent != NULL) {
				for (auto it : parent->components) {
					engine::Component *c = it.second;
					if (typeid(T).name() == c->type) {
						return static_cast<T*>(c);
					}
				}
				parent->getComponentInParents<T>();
			}
			return NULL;
		}
		template<typename T> T* getComponentInChildren() {
			for(auto cit : children) {
				engine::GameObject *child = cit.second;
				for (auto it : child->components) {
					engine::Component *c = it.second;
					if (typeid(T).name() == c->type) {
						return static_cast<T*>(c);
					}
				}
				child->getComponentInChildren<T>();
			}
			return NULL;
		}
		template<typename T> std::map<ullong, T*> getComponents() {
			std::map<ullong, T*> components;
			for (auto it : this->components) {
				engine::Component *c = it.second;
				if (typeid(T).name() == c->type)
					components[it.first] = (static_cast<T*>(c));
			}
			return components;
		}
		//a component can't exist outside a game object, so this will only receive new references
		template<typename T> bool addComponent(T* component) {
			if (!component->allowMultiple && hasComponent<T>()) {
				delete component;
				return false;
			}
			ullong id = getFirstAvailableMapIndex(this -> components);
			this->components[id] = component;
			component->gameObject = this;
			component->setup();
			return true;
		}
		bool constructComponent(boost::property_tree::ptree node);
		//don't call a component destructor separate from the functions in this file
		//only call this function if you know for sure the component is unique
		template<typename T> void removeComponent() {
			for (auto it : components) {
				ullong i = it;
				if (components[i]->type == typeid(T).name()) {
					T* component = static_cast<T*>(components[i]);
					delete component;
					components.erase(i);
					break;
				}
			}
		}
		template<typename T> void removeComponent(T* component) {
			components.erase(std::remove(components.begin(), components.end(), component), components.end());
			delete component;
		}
	};
}

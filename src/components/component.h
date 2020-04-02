#pragma once
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class GameObject;

	class Component {
	public:
		std::string type = "change_me";
		std::string name = "Component"; //change this for serialization purposes to actual class names, since typeid is compiler specific
		bool allowMultiple = false; // specifies wheter a game object can have multiple components of the same type
		GameObject* gameObject;

		Component();
		Component(bool allowMultiple);
		Component(const Component& component);
		virtual ~Component();
		virtual boost::property_tree::ptree serialize() = 0;
		virtual void setup() = 0;
	protected:
		bool initialized;
		virtual void setType() = 0; //you can also set the name in here
	};
}

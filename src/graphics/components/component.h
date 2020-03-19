#pragma once
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class GameObject;

	class Component {
	public:
		std::string type;
		bool allowMultiple = false; // specifies wheter a game object can have multiple components of the same type
		GameObject* gameObject;

		Component();
		Component(bool allowMultiple);
		virtual ~Component();
		virtual boost::property_tree::ptree serialize();
		virtual void deserialize();
		virtual Component* instantiate();
		virtual void setup();
	protected:
		bool initialized;
		virtual void setType();
	};
}
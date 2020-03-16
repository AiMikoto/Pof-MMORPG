#pragma once
#include "constants.h"
#include <boost/property_tree/ptree.hpp>

namespace graphics {
	class GameObject;

	class Component {
	public:
		uint type;
		GameObject* gameObject;

		Component();
		~Component();

		virtual boost::property_tree::ptree serialize();
		virtual void deserialize();
		virtual Component* instantiate(GameObject* gameObject);
	};
}
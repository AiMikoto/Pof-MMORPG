#pragma once
#include "scene/objects.h"
#include "components/component.h"
#include "core/constants.h"
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class MeshFilter : public Component {
	public:
		uint modelID;

		MeshFilter();
		MeshFilter(const MeshFilter& meshFilter);
		~MeshFilter();

		void setType();
		void setup();
		boost::property_tree::ptree serialize();
		MeshFilter* deserialize(boost::property_tree::ptree node);
		bool hasModel();
	};
}
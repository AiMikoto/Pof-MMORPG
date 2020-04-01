#pragma once
#include "scene/objects.h"
#include "components/component.h"
#include "core/constants.h"
#include <boost/property_tree/ptree.hpp>

namespace engine {
	class MeshFilter : public Component {
	public:
		uint modelID, defaultModelID;
		std::string modelPath, defaultModelPath;

		MeshFilter();
		MeshFilter(const MeshFilter& meshFilter);
		MeshFilter(boost::property_tree::ptree node);
		~MeshFilter();

		void setup();
		boost::property_tree::ptree serialize();
		bool hasModel();
	protected:
		void setType();
	};
}
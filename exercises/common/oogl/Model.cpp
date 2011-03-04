/*
 * Model.cpp
 *
 *  Created on: 17.02.2011
 *      Author: sam
 */

#include <cassert>
#include <stdexcept>
#include <oogl/Model.h>
#include <oogl/model/Model3ds.h>

#include <utils/log.h>

namespace oogl {

Model::Model(const std::string fileName): fileName(fileName) {

}

Model::~Model() {

}


Model* loadModel(const std::string& fileName) {
	std::string ext = "";
	if(fileName.find_last_of(".") != std::string::npos)
		ext = fileName.substr(fileName.find_last_of(".")+1);

	if(ext == "3ds" || ext == "3DS" || ext == "3Ds" || ext == "3dS") {
		LOG_DEBUG << "loading 3ds model " << fileName << std::endl;
		return new model::Model3ds(fileName);
	}
	throw std::runtime_error("unsupported model type: "+fileName);
}

}

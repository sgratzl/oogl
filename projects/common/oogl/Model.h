/*
 * Model.h
 *
 *  Created on: 17.02.2011
 *      Author: sam
 */

#pragma once

#include <glm/glm.hpp>

#include <string>

#include <memory>

namespace oogl {

struct BoundingBox {
	glm::vec3 min, max;
};

struct BoundingSphere {
	float radius;
	glm::vec3 center;
};

class Model {
public:
	enum RenderOption {
		RENDER_NO_TEXTURES = 1<<0,
		RENDER_NO_MATERIALS = 1<<1,
		RENDER_NO_CULL_FACE = 1<<2,
		RENDER_NO_DISLAYLIST = 1 << 3
	};
	typedef unsigned int RenderOptions;

	enum LoadOption {
		LOAD_NO_NORMALIZATION = 1 << 0,
		LOAD_NORMALIZE_TWO = 1 << 1,
		LOAD_SET_SMOOTHING_GROUP = 1 << 2
	};
	typedef unsigned int LoadOptions;

	Model(const std::string fileName,Model::LoadOptions options);
	virtual ~Model();

	virtual void render(RenderOptions options = 0) = 0;

	std::string getFileName() const {
		return fileName;
	}

	BoundingBox getBoundingBox() const {
		return boundingBox;
	}

	BoundingSphere getBoundingSphere() const {
		BoundingSphere bs;
		bs.center = (boundingBox.min + boundingBox.max) * 0.5f;
		bs.radius = (glm::max(boundingBox.max.x-boundingBox.min.x, glm::max(boundingBox.max.y-boundingBox.min.y, boundingBox.max.z-boundingBox.min.z))) * 0.5f;
		return bs;
	}

protected:
	BoundingBox boundingBox;
	std::string fileName;
	LoadOptions loadOptions;
};

Model* loadModel(const std::string& fileName, Model::LoadOptions options = 0);

}


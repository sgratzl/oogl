/*
 * Model.h
 *
 *  Created on: 17.02.2011
 *      Author: sam
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <glm/glm.hpp>

#include <string>

#include <memory>
#ifndef WIN32
# include <tr1/memory>
#endif

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
	Model(const std::string fileName);
	virtual ~Model();

	virtual void render() = 0;

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
};

typedef std::shared_ptr<Model> ModelPtr;

ModelPtr loadModel(const std::string& fileName);

}

#endif /* MODEL_H_ */

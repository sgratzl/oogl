/*
 * Texture3D.h
 *
 *  Created on: 21.05.2011
 *      Author: sam
 */

#ifndef TEXTURE3D_H_
#define TEXTURE3D_H_

#include <utils/log.h>

#include <utility>
#include <memory>

#include <oogl/glIncludes.h>
#include <glm/glm.hpp>

namespace oogl {

class Texture3D {
public:
	static Texture3D* load(const std::string& fileName);

	virtual ~Texture3D();

	glm::uint getWidth() {
		return width;
	}
	glm::uint getHeight() {
		return height;
	}
	glm::uint getDepth() {
		return depth;
	}

	GLint getFormat() {
		return format;
	}
	glm::uvec3 getDimensions() {
		return glm::uvec3(width, height, depth);
	}

	std::string getName() const {
		return name;
	}

	void bind(glm::uint toTexture = 0);
	void unbind();

	void render();


protected:
	friend class GLSLAttrib;
	friend class FrameBufferObject;

	Texture3D(const std::string& name, const glm::uvec3& dim, const GLuint textureId, GLint format);

	bool isBound() const {
		return bindedTexture >= 0;
	}
private:
	std::string name;
	GLuint textureId;
	glm::uint width, height, depth;
	GLint format;
	int bindedTexture;
};

Texture3D* loadTexture3D(const std::string& fileName);

}

#endif /* TEXTURE3D_H_ */

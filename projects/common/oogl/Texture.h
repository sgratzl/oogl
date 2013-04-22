/*
 * Texture.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#pragma once

#include <utils/log.h>

#include <utility>
#include <memory>

#include <oogl/glIncludes.h>
#include <glm/glm.hpp>

namespace oogl {

class Texture {
public:
	virtual ~Texture();

	GLint getFormat() {
		return format;
	}

	std::string getName() const {
		return name;
	}

	virtual void bind(glm::uint toTexture = 0);
	virtual void unbind();

	virtual void render() = 0;

protected:
	friend class GLSLAttrib;
	friend class FrameBufferObject;

	Texture(const std::string& name, const GLuint textureId, GLint format, GLenum textureType);

	bool isBound() const {
		return bindedTexture >= 0;
	}
private:
	std::string name;
	GLuint textureId;
	GLint format;
	GLenum textureType;
	int bindedTexture;
};

Texture* loadTexture(const std::string& fileName);

}


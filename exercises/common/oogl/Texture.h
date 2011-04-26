/*
 * Texture.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <utils/log.h>

#include <utility>
#include <memory>

#include <oogl/glIncludes.h>
#include <glm/glm.hpp>


namespace oogl {

class Texture {
public:
	static Texture* createColor(const glm::uvec2& dim, const GLint format = GL_RGBA);
	static Texture* createDepth(const glm::uvec2& dim, const GLint format = GL_DEPTH_COMPONENT);
	static Texture* loadTexture(const std::string& fileName);

	virtual ~Texture();

	glm::uint getWidth() {
		return width;
	}
	glm::uint getHeight() {
		return height;
	}
	GLint getFormat() {
		return format;
	}
	glm::uvec2 getDimensions() {
		return glm::uvec2(width, height);
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

	Texture(const std::string& name, const glm::uvec2& dim, const GLuint textureId, GLint format);

	bool isBound() const {
		return bindedTexture >= 0;
	}
private:
	std::string name;
	GLuint textureId;
	glm::uint width, height;
	GLint format;
	int bindedTexture;
};

Texture* loadTexture(const std::string& fileName);


}
#endif /* TEXTURE_H_ */

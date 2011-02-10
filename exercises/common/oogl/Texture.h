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

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;

class Texture {
	friend class GLSLAttrib;
	friend class FrameBufferObject;
public:
	static TexturePtr createColor(const glm::uvec2& dim, const GLint format = GL_RGBA);
	static TexturePtr createDepth(const glm::uvec2& dim, const GLint format = GL_DEPTH_COMPONENT24);

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

	void bind();
	void unbind();

protected:
	Texture(const glm::uvec2& dim, const GLuint textureId, GLint format);

private:
	GLuint textureId;
	glm::uint width, height;
	GLint format;
};

}
#endif /* TEXTURE_H_ */

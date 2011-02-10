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

namespace oogl {

class Texture;
typedef boost::shared_ptr<Texture> TexturePtr;

class Texture {
friend class GLSLAttrib;
friend class FrameBuffer;
friend class TextureAccessorTrait;
public:
	static TexturePtr createColor(const OpenGLContextPtr& context,const uvec2& dim,const GLint format=GL_RGBA);
	static TexturePtr createDepth(const OpenGLContextPtr& context,const uvec2& dim,const GLint format=GL_DEPTH_COMPONENT24);

	virtual ~Texture();

	inline uint getWidth() { return width; }
	inline uint getHeight() { return height; }
	inline GLint getFormat() { return format; }
	inline uvec2 getDimensions() { return uvec2(width, height); }

	inline void bind() {
		glBindTexture(GL_TEXTURE_2D, textureId);
	}
	inline void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:
	GLuint textureId;
	uint width, height;
	GLint format;
	inline GLuint getId() const { return textureId; }

	LOG_DECLARE_LOGGER

	Texture(const OpenGLContextPtr& context,const uvec2& dim,const GLuint textureId, GLint format);
};

class TextureAccessorTrait {
protected:
	inline int getTextureId(const TexturePtr& texture) const { return texture->getId(); }
};

}
#endif /* TEXTURE_H_ */

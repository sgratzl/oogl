/*
 * Texture.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <openglwrapper/Texture.h>
#include <openglwrapper/gl_error.h>

namespace openglwrapper {

LOG_DEFINE_LOGGER(Texture, openglwrapper.Texture)

TexturePtr Texture::createColor(const OpenGLContextPtr& context,const uvec2& dim,const GLint format) {
	LOG_DEBUG("create color texture: " << dim.x << " " << dim.y);
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl_check_error();

	TexturePtr tex(new Texture(context, dim, textureId, format));
	tex->unbind();
	return tex;
}

TexturePtr Texture::createDepth(const OpenGLContextPtr& context,const uvec2& dim,const GLint format) {
	LOG_DEBUG("create depth texture: " << dim.x << " " << dim.y);
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	gl_check_error();

	TexturePtr tex(new Texture(context, dim, textureId, format));
	tex->unbind();
	return tex;
}


Texture::Texture(const OpenGLContextPtr& context,const uvec2& dim,const GLuint textureId, GLint format):
		width(dim.x), height(dim.y), textureId(textureId), format(format) {

}

Texture::~Texture() {
	if(textureId)
		glDeleteTextures(1, &textureId);
}

}

/*
 * Texture.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/Texture.h>
#include <oogl/gl_error.h>

#include <glm/glm_ostream.hpp>

namespace oogl {


TexturePtr Texture::createColor(const glm::uvec2& dim,const GLint format) {
	LOG_DEBUG << "create color texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOG_GL_ERRORS();

	TexturePtr tex(new Texture(dim, textureId, format));
	tex->unbind();
	return tex;
}

TexturePtr Texture::createDepth(const glm::uvec2& dim,const GLint format) {
	LOG_DEBUG << "create depth texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	LOG_GL_ERRORS();

	TexturePtr tex(new Texture(dim, textureId, format));
	tex->unbind();
	return tex;
}


Texture::Texture(const glm::uvec2& dim,const GLuint textureId, GLint format):
		width(dim.x), height(dim.y), textureId(textureId), format(format) {

}

Texture::~Texture() {
	if(textureId)
		glDeleteTextures(1, &textureId);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

}

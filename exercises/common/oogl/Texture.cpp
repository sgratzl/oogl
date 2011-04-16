/*
 * Texture.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/Texture.h>
#include <oogl/gl_error.h>
#include <oogl/Image.h>

#include <exception>
#include <sstream>
#include <stdexcept>

#include <glm/glm_ostream.hpp>

namespace oogl {

Texture* Texture::createColor(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create color texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture* tex = new Texture("generated",dim, textureId, format);
	tex->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOG_GL_ERRORS();

	tex->unbind();
	return tex;
}

Texture* Texture::createDepth(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create depth texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture* tex = new Texture("generated",dim, textureId, format);
	tex->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	tex->unbind();
	LOG_GL_ERRORS();
	return tex;
}

Texture* Texture::loadTexture(const std::string& fileName) {
	LOG_DEBUG << "load texture: " << fileName << std::endl;
	std::auto_ptr<oogl::Image> image(oogl::loadImage(fileName));

	GLuint textureId;
	glGenTextures(1, &textureId);
	Texture* tex = new Texture(fileName, image->getDimensions(), textureId, image->getFormat());
	tex->bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture interpolation method to use linear interpolation (no MIPMAPS)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D,
		0, //mip map level 0..top
		GL_RGBA, //internal format of the data in memory
		image->getWidth(),
		image->getHeight(),
		0,//border width in pixels (can either be 1 or 0)
		image->getFormat(),	// Image format (i.e. RGB, RGBA, BGR etc.)
		image->getType(),// Image data type
		image->getData());// The actual image data itself

	tex->unbind();

	LOG_DEBUG << "loaded texture: " << fileName << image->getDimensions() << std::endl;
	LOG_GL_ERRORS();
	return tex;
}

Texture::Texture(const std::string& name, const glm::uvec2& dim, const GLuint textureId, GLint format) :
	name(name), width(dim.x), height(dim.y), textureId(textureId), format(format), bindedTexture(-1) {

}

Texture::~Texture() {
	LOG_DEBUG << "free texture " << name << std::endl;
	if (textureId)
		glDeleteTextures(1, &textureId);
}

void Texture::bind(glm::uint toTexture) {
	bindedTexture = toTexture;
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + toTexture);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void Texture::unbind() {
	if(bindedTexture < 0)
		return;
	glActiveTexture(GL_TEXTURE0 + bindedTexture);
	glBindTexture(GL_TEXTURE_2D, 0);
	bindedTexture = -1;
}

Texture* loadTexture(const std::string& fileName) {
	return Texture::loadTexture(fileName);
}

}

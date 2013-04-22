/*
 * Texture.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/Texture.h>
#include <oogl/Texture2D.h>
#include <oogl/Texture3D.h>
#include <oogl/gl_error.h>
#include <oogl/Image.h>

#include <exception>
#include <sstream>
#include <stdexcept>

#include <glm/glm_ostream.hpp>

namespace oogl {

Texture::Texture(const std::string& name, const GLuint textureId, GLint format, GLenum textureType) :
	name(name), textureId(textureId), format(format), textureType(textureType), bindedTexture(-1) {

}

Texture::~Texture() {
	LOG_DEBUG << "free texture " << name << std::endl;
	if (textureId)
		glDeleteTextures(1, &textureId);
}

void Texture::bind(glm::uint toTexture) {
	if(toTexture >= GL_TEXTURE0)
		toTexture -= GL_TEXTURE0;
	bindedTexture = toTexture;
	//glEnable(textureType);
	glActiveTexture(GL_TEXTURE0 + toTexture);
	glBindTexture(textureType, textureId);
}

void Texture::unbind() {
	if(bindedTexture < 0)
		return;
	glActiveTexture(GL_TEXTURE0 + bindedTexture);
	glBindTexture(textureType, GL_NONE);
	bindedTexture = -1;
}

Texture* loadTexture(const std::string& fileName) {
	LOG_DEBUG << "load texture: " << fileName << std::endl;
	Image *image = oogl::loadImage(fileName);

	Texture* tex = NULL;
	if(image->getDepth() <= 1)
		tex = Texture2D::load(image);
	else
		tex = Texture3D::load(image);

	delete image;
	return tex;
}

}

/*
 * Texture.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/Texture.h>
#include <oogl/gl_error.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

#include <exception>
#include <sstream>
#include <stdexcept>

#include <glm/glm_ostream.hpp>

#define LOG_DEVIL_ERRORS() \
	{\
		for(ILenum err = ilGetError(); err != IL_NO_ERROR; err = ilGetError()) { \
			LOG_ERROR << "DevIL_ERROR: " << iluErrorString(err) << std::endl; \
		}\
	}

namespace oogl {

TexturePtr Texture::createColor(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create color texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	LOG_GL_ERRORS();

	TexturePtr tex(new Texture("generated",dim, textureId, format));
	tex->unbind();
	return tex;
}

TexturePtr Texture::createDepth(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create depth texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	LOG_GL_ERRORS();

	TexturePtr tex(new Texture("generated",dim, textureId, format));
	tex->unbind();
	return tex;
}

TexturePtr Texture::loadTexture(const std::string& fileName) {
	LOG_INFO << "load texture: " << fileName << std::endl;

	static bool ilInitialized = false;
	if(!ilInitialized) {
		LOG_DEBUG << "initialize DevIL" << std::endl;
		ilInit();
		iluInit();
		ilutInit();
		ilutRenderer(ILUT_OPENGL);
		LOG_DEVIL_ERRORS()
		ilInitialized = true;
	}

	ILuint img;
	ilGenImages(1, &img);
	ilBindImage(img);
	if (!ilLoadImage(fileName.c_str())) {
		LOG_ERROR << "can't load image: " << fileName << std::endl;
		LOG_DEVIL_ERRORS()
		ilDeleteImages(1,&img);
		throw std::runtime_error("can't load image: "+fileName);
	}

	GLuint textureId = ilutGLBindTexImage();
	LOG_GL_ERRORS();

	glm::uvec2 dim(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
	TexturePtr tex(new Texture(fileName, glm::uvec2(dim.x,dim.y), textureId, ilGetInteger(IL_IMAGE_FORMAT)));
	tex->unbind();

	ilDeleteImages(1, &img);
	LOG_DEVIL_ERRORS();

	LOG_INFO << "loaded texture: " << fileName << dim << std::endl;

	return tex;
}

Texture::Texture(const std::string& name, const glm::uvec2& dim, const GLuint textureId, GLint format) :
	name(name), width(dim.x), height(dim.y), textureId(textureId), format(format), bindedTexture(-1) {

}

Texture::~Texture() {
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
	glBindTexture(GL_TEXTURE_2D, 0);
	bindedTexture = -1;
}

TexturePtr loadTexture(const std::string& fileName) {
	return Texture::loadTexture(fileName);
}

}

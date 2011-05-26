/*
 * Texture2D.cpp
 *
 *  Created on: 26.05.2011
 *      Author: sam
 */

#include <oogl/Texture2D.h>


#include <oogl/Texture.h>
#include <oogl/gl_error.h>
#include <oogl/Image.h>

#include <exception>
#include <sstream>
#include <stdexcept>

#include <glm/glm_ostream.hpp>

namespace oogl {

Texture2D* Texture2D::createColor(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create color texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture2D* tex = new Texture2D("generated",dim, textureId, format);
	tex->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	LOG_GL_ERRORS();

	tex->unbind();
	return tex;
}

Texture2D* Texture2D::createDepth(const glm::uvec2& dim, const GLint format) {
	LOG_DEBUG << "create depth texture: " << dim << std::endl;
	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture2D* tex = new Texture2D("generated",dim, textureId, format);
	tex->bind();

	glTexImage2D(GL_TEXTURE_2D, 0, format, dim.x, dim.y, 0 /*no border*/, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

	LOG_GL_ERRORS();
	tex->unbind();

	return tex;
}

Texture2D* Texture2D::load(Image* image) {
	if(image->getDepth() > 1) {
		LOG_ERROR << "can only handle 2D Images, but " << image->getName() << " has a depth of " << image->getDepth() << std::endl;
		return NULL;
	}

	GLuint textureId;
	glGenTextures(1, &textureId);
	Texture2D* tex = new Texture2D(image->getName(), glm::uvec2(image->getWidth(), image->getHeight()), textureId, image->getFormat());
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

	LOG_DEBUG << "loaded texture: " << image->getName() << image->getDimensions() << std::endl;
	LOG_GL_ERRORS();
	return tex;
}

Texture2D::Texture2D(const std::string& name, const glm::uvec2& dim, const GLuint textureId, GLint format) :
	Texture(name, textureId, format, GL_TEXTURE_2D), dim(dim) {

}

Texture2D::~Texture2D() {

}

void Texture2D::render() {
	glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	const GLfloat c_vertices[]  = {-1,-1, +1,-1, +1,+1, -1,+1};
	const GLfloat c_texcoords[] = {+0,+0, +1,+0, +1,+1, +0,+1};

	glVertexPointer(2, GL_FLOAT, 0, c_vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, c_texcoords);
	glDrawArrays(GL_QUADS, 0, 4);

	unbind();
	glPopClientAttrib();
	glPopAttrib();
}

}

/*
 * FrameBufferObject.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/FrameBufferObject.h>
#include <cassert>
#include <stdexcept>

#include "gl_error.h"

namespace oogl {

FrameBufferObject* FrameBufferObject::create(glm::uvec2 dim, const unsigned textureCount, const GLint textureFormat, const GLint depthFormat) {
	assert(textureCount <= 4);

	std::vector<Texture*> textures;
	for(unsigned i = 0; i< textureCount; ++i) {
		textures.push_back(Texture::createColor(dim, textureFormat));
	}

	Texture* depthTexture = NULL;
	if(depthFormat > 0) {
		depthTexture = Texture::createDepth(dim, depthFormat);
	}

	return new FrameBufferObject(dim, textures, depthTexture);
}

FrameBufferObject* FrameBufferObject::createDepthOnly(glm::uvec2 dim, const GLint depthFormat) {
	return create(dim, 0, 0, depthFormat);
}

FrameBufferObject::FrameBufferObject(glm::uvec2 dim, const std::vector<Texture*>& textures, Texture* depthTexture) :
	dim(dim), textures(textures), depthTexture(depthTexture) {
	LOG_DEBUG << "create fbo: " << dim.x << " " << dim.y << std::endl;

	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	LOG_GL_ERRORS();

	if (depthTexture) {
		assert (dim.x == depthTexture->getWidth() && dim.y == depthTexture->getHeight());
		LOG_DEBUG << "attach depth texture" << std::endl;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthTexture->textureId, 0);
	}

	LOG_DEBUG << "attach textures" << std::endl;
	// attach
	assert(textures.size() <= 15);
	for (unsigned i = 0; i < textures.size(); ++i) {
		Texture* tex = textures[i];
		assert (dim.x == tex->getWidth() && dim.y == tex->getHeight());
		LOG_DEBUG << "attach color texture " << i << std::endl;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+i, GL_TEXTURE_2D, tex->textureId, 0);
	}

	if(textures.size() <= 0) {
		// instruct openGL that we won't bind a color texture with the currently binded FBO
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	LOG_DEBUG << "check errors" << std::endl;
	checkError();
	LOG_DEBUG << "checked errors" << std::endl;

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	LOG_GL_ERRORS();
}

void FrameBufferObject::checkError() {
	GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status == GL_FRAMEBUFFER_COMPLETE_EXT) {
		LOG_DEBUG << "no errors" << std::endl;
		return;
	}

	switch (status) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_FORMATS");
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_UNSUPPORTED" << std::flush << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED");
	default:
		LOG_ERROR << "unknown fbo error: " << status << std::flush << std::endl;
		throw std::runtime_error("unknown framebuffer error");
	}
}

FrameBufferObject::~FrameBufferObject() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	for(std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		delete *it;
	textures.clear();

	if(depthTexture != NULL)
		delete depthTexture;

	if (fb)
		glDeleteFramebuffersEXT(1, &fb);
}

void FrameBufferObject::beginCommon() {
	glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT | GL_PIXEL_MODE_BIT);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
	glViewport(0, 0, dim.x, dim.y);
	LOG_GL_ERRORS();
}

void FrameBufferObject::begin(int target) {
	beginCommon();

	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT+target);
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT+target);
	LOG_GL_ERRORS();
}
void FrameBufferObject::beginDepth() {
	beginCommon();

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	LOG_GL_ERRORS();
}
void FrameBufferObject::beginAll() {
	beginCommon();

	GLenum* buffer = new GLenum[textures.size()];
	for(unsigned i = 0; i < textures.size(); ++i) {
		buffer[i] = GL_COLOR_ATTACHMENT0_EXT+i;
	}
	glDrawBuffers(textures.size(), buffer);

	delete[] buffer;
	LOG_GL_ERRORS();

}
void FrameBufferObject::end() {
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glPopAttrib();
}

}

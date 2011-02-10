/*
 * FrameBufferObject.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/FrameBufferObject.h>
#include <cassert>

namespace oogl {

const GLenum buffer[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

FrameBufferObjectPtr FrameBufferObject::create(glm::uvec2 dim, const unsigned textureCount, const GLint textureFormat, const GLint depthFormat) {
	assert(textureCount <= 4);

	std::vector<TexturePtr> textures;
	for(unsigned i = 0; i< textureCount; ++i) {
		textures.push_back(Texture::createColor(dim, textureFormat));
	}

	TexturePtr depthTexture;
	if(depthFormat > 0) {
		depthTexture = Texture::createDepth(dim, depthFormat);
	}

	return FrameBufferObjectPtr(new FrameBufferObject(dim, textures, depthTexture));
}

FrameBufferObjectPtr FrameBufferObject::createDepthOnly(glm::uvec2 dim, const GLint depthFormat) {
	std::vector<TexturePtr> textures;

	TexturePtr depthTexture = Texture::createDepth(dim, depthFormat);

	return FrameBufferObjectPtr(new FrameBufferObject(dim, textures, depthTexture));
}

FrameBufferObject::FrameBufferObject(glm::uvec2 dim, const std::vector<TexturePtr>& textures, TexturePtr depthTexture) :
	dim(dim), textures(textures), depthTexture(depthTexture) {
	LOG_DEBUG << "create fbo: " << dim.x << " " << dim.y << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0); //!!important

	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	if (depthTexture) {
		assert (dim.x == depthTexture->getWidth() && dim.y == depthTexture->getHeight());
		LOG_DEBUG << "attach depth texture" << std::endl;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->textureId, 0);
	}

	LOG_DEBUG << "attach textures" << std::endl;
	// attach
	assert(textures.size() <= 4);
	for (unsigned i = 0; i < textures.size(); ++i) {
		TexturePtr tex = textures[i];
		assert (dim.x == tex->getWidth() && dim.y == tex->getHeight());
		LOG_DEBUG << "attach color texture " << i << std::endl;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, buffer[i], GL_TEXTURE_2D, tex->textureId, 0);
	}

	LOG_DEBUG << "check errors" << std::endl;
	checkError();

	end();
}

void FrameBufferObject::checkError() {
	GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
		return;

	switch (status) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_FORMATS");
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
	case GL_FRAMEBUFFER_UNSUPPORTED:
		LOG_ERROR << "can't create fbo: " << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
		throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED");
	default:
		throw std::runtime_error("unknown framebuffer error");
	}
}

FrameBufferObject::~FrameBufferObject() {
	end();

	textures.clear();

	if (fb)
		glDeleteFramebuffersEXT(1, &fb);
}

void FrameBufferObject::begin(int target) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffer(buffer[target]);
	glReadBuffer(buffer[target]);
}
void FrameBufferObject::beginDepth() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}
void FrameBufferObject::beginAll() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffers(textures.size(), buffer);
}
void FrameBufferObject::end() {
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}
}

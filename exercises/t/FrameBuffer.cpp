/*
 * FrameBuffer.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <openglwrapper/FrameBuffer.h>
#include <boost/foreach.hpp>
#include <cassert>

namespace oogl {
LOG_DEFINE_LOGGER(FrameBuffer, openglwrapper.FrameBuffer)
const GLenum buffer[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

FrameBufferPtr FrameBuffer::create(const OpenGLContextPtr& context, uvec2 dim, const unsigned textureCount, const GLint textureFormat, const GLint depthFormat) {
	assert(textureCount <= 4);

	std::vector<TexturePtr> textures;
	for(unsigned i = 0; i< textureCount; ++i) {
		textures.push_back(Texture::createColor(context, dim, textureFormat));
	}

	TexturePtr depthTexture;
	if(depthFormat > 0) {
		depthTexture = Texture::createDepth(context, dim, depthFormat);
	}

	return FrameBufferPtr(new FrameBuffer(context, dim, textures, depthTexture));
}

FrameBufferPtr FrameBuffer::createDepthOnly(const OpenGLContextPtr& context, uvec2 dim, const GLint depthFormat) {
	std::vector<TexturePtr> textures;

	TexturePtr depthTexture = Texture::createDepth(context, dim, depthFormat);

	return FrameBufferPtr(new FrameBuffer(context, dim, textures, depthTexture));
}

FrameBuffer::FrameBuffer(const OpenGLContextPtr& context, uvec2 dim, const std::vector<TexturePtr>& textures, TexturePtr depthTexture) :
	dim(dim), textures(textures), depthTexture(depthTexture) {
	LOG_DEBUG("create fbo: " << dim.x << " " << dim.y);

	glBindTexture(GL_TEXTURE_2D, 0); //!!important

	glGenFramebuffersEXT(1, &fb);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	if (depthTexture) {
		assert (dim.x == depthTexture->getWidth() && dim.y == depthTexture->getHeight());
		LOG_DEBUG("attach depth texture");
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->getId(), 0);
	}

	LOG_DEBUG("attach textures");
	// attach
	assert(textures.size() <= 4);
	for (unsigned i = 0; i < textures.size(); ++i) {
		TexturePtr tex = textures[i];
		assert (dim.x == tex->getWidth() && dim.y == tex->getHeight());
		LOG_DEBUG("attach color texture " << i);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER, buffer[i], GL_TEXTURE_2D, tex->getId(), 0);
	}



	LOG_DEBUG("check errors");
	checkError();

	end();
}

void FrameBuffer::checkError() {
	GLuint status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER);
	if (status == GL_FRAMEBUFFER_COMPLETE)
		return;

	switch (status) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_FORMATS");
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
		throw std::runtime_error("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
	case GL_FRAMEBUFFER_UNSUPPORTED:
		LOG_ERROR("can't create fbo: " << "GL_FRAMEBUFFER_UNSUPPORTED");
		throw std::runtime_error("GL_FRAMEBUFFER_UNSUPPORTED");
	default:
		throw std::runtime_error("unknown framebuffer error");
	}
}

FrameBuffer::~FrameBuffer() {
	end();

	textures.clear();

	if (fb)
		glDeleteFramebuffersEXT(1, &fb);
}

void FrameBuffer::begin(int target) {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);
	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffer(buffer[target]);
	glReadBuffer(buffer[target]);
}
void FrameBuffer::beginDepth() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}
void FrameBuffer::beginAll() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER, fb);

	glViewport(0, 0, dim.x, dim.y);
	glDrawBuffers(textures.size(), buffer);
}
void FrameBuffer::end() {
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}
}

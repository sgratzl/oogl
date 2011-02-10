/*
 * FrameBuffer.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <openglwrapper/Texture.h>
#include <utils/log.h>


namespace oogl {

class FrameBuffer;
typedef boost::shared_ptr<FrameBuffer> FrameBufferPtr;

class FrameBuffer {
public:
	static FrameBufferPtr create(const OpenGLContextPtr& context, uvec2 dim, const unsigned textureCount = 1, const GLint textureFormat=GL_RGBA, const GLint depthFormat = -1);
	static FrameBufferPtr createDepthOnly(const OpenGLContextPtr& context, uvec2 dim, const GLint depthFormat=GL_DEPTH_COMPONENT24);

	virtual ~FrameBuffer();

	void begin(int target =0);
	void beginDepth();
	void beginAll();
	void end();

	inline TexturePtr getTexture(int target = 0) { return textures[target]; }
	inline TexturePtr getDepthTexture() { return depthTexture; };

protected:
	FrameBuffer(const OpenGLContextPtr& context, uvec2 dim, const std::vector<TexturePtr>& textures, TexturePtr depthTexture);
private:
	uvec2 dim;
	std::vector<TexturePtr> textures;
	TexturePtr depthTexture;
	GLuint fb, drb;
	LOG_DECLARE_LOGGER

	void checkError();
};

}

#endif /* FRAMEBUFFER_H_ */

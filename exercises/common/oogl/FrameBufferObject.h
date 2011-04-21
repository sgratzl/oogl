/*
 * FrameBufferObject.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef FRAMEBUFFEROBJECT_H_
#define FRAMEBUFFEROBJECT_H_


#include <oogl/glIncludes.h>

#include <oogl/Texture.h>
#include <utils/log.h>

#include <vector>
#include <memory>


namespace oogl {

class FrameBufferObject {
public:
	static FrameBufferObject* create(glm::uvec2 dim, const unsigned textureCount = 1, const GLint textureFormat=GL_RGBA, const GLint depthFormat = -1);
	static FrameBufferObject* createDepthOnly(glm::uvec2 dim, const GLint depthFormat=GL_DEPTH_COMPONENT24);

	virtual ~FrameBufferObject();

	void begin(int target =0);
	void beginDepth();
	void beginAll();
	void end();

	Texture* getTexture(int target = 0) { return textures[target]; }
	Texture* getDepthTexture() { return depthTexture; };

protected:
	FrameBufferObject(glm::uvec2 dim, const std::vector<Texture*>& textures, Texture* depthTexture);

private:
	glm::uvec2 dim;
	std::vector<Texture*> textures;
	Texture* depthTexture;
	GLuint fb, drb;

	void beginCommon();
	void checkError();
};

}

#endif /* FRAMEBUFFEROBJECT_H_ */

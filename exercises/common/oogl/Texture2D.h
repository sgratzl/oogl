/*
 * Texture2D.h
 *
 *  Created on: 26.05.2011
 *      Author: sam
 */

#ifndef TEXTURE2D_H_
#define TEXTURE2D_H_

#include <oogl/Texture.h>
#include <oogl/Image.h>

namespace oogl {

class Texture2D: public Texture {
public:
	static Texture2D* load(Image *image);
	static Texture2D* createDepth(const glm::uvec2& dim, const GLint format);
	static Texture2D* createColor(const glm::uvec2& dim, const GLint format);

	virtual ~Texture2D();

	glm::uint getWidth() {
		return dim.x;
	}

	glm::uint getHeight() {
		return dim.y;
	}

	glm::uvec2 getDimensions() {
		return dim;
	}

	void render();

protected:
	Texture2D(const std::string& name, const glm::uvec2& dim, const GLuint textureId, GLint format);

private:
	glm::uvec2 dim;
};

}

#endif /* TEXTURE2D_H_ */

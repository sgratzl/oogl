/*
 * Texture3D.h
 *
 *  Created on: 21.05.2011
 *      Author: sam
 */

#ifndef TEXTURE3D_H_
#define TEXTURE3D_H_

#include <oogl/Texture.h>
#include <oogl/Image.h>

namespace oogl {

class Texture3D: public Texture {
public:
	static Texture3D* load(Image *image);

	virtual ~Texture3D();

	glm::uint getWidth() {
		return dim.x;
	}

	glm::uint getHeight() {
		return dim.y;
	}

	glm::uint getDepth() {
		return dim.z;
	}

	glm::uvec3 getDimensions() {
		return dim;
	}

	void render();

protected:
	Texture3D(const std::string& name, const glm::uvec3& dim, const GLuint textureId, GLint format);

private:
	glm::uvec3 dim;
};

}

#endif /* TEXTURE3D_H_ */

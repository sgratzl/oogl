/*
 * Image.h
 *
 *  Created on: 31.03.2011
 *      Author: sam
 */

#pragma once

#include <string>
#include <oogl/glIncludes.h>
#include <glm/glm.hpp>

namespace oogl {

class Image {
public:
	virtual ~Image();

	static Image* load(const std::string &fileName);

	glm::uint getWidth();
	glm::uint getHeight();
	glm::uint getDepth();

	int getFormat();
	int getType();
	int getBytesPerPixel();

	glm::uvec3 getDimensions() {
		return glm::uvec3(getWidth(), getHeight(), getDepth());
	}

	std::string getName() {
		return fileName;
	}

	unsigned char* getData();

protected:
	std::string fileName;
	unsigned int img;

	Image(const std::string& fileName, unsigned int img);

	static void initDevIL();
};

Image* loadImage(const std::string &fileName);

}


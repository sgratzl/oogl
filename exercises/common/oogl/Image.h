/*
 * Image.h
 *
 *  Created on: 31.03.2011
 *      Author: sam
 */

#ifndef IMAGE_H_
#define IMAGE_H_

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

	int getFormat();
	int getType();
	int getBytesPerPixel();

	glm::uvec2 getDimensions() {
		return glm::uvec2(getWidth(), getHeight());
	}

	unsigned char* getData();

private:
	std::string fileName;
	unsigned int img;

	Image(const std::string& fileName, unsigned int img);

	static void initDevIL();


};

Image* loadImage(const std::string &fileName);

}

#endif /* IMAGE_H_ */

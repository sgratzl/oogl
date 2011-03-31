/*
 * Image.cpp
 *
 *  Created on: 31.03.2011
 *      Author: sam
 */

#include <oogl/Image.h>
#include <oogl/gl_error.h>

#include <IL/il.h>
#include <IL/ilu.h>
//#include <IL/ilut.h>

#include <utils/log.h>

namespace oogl {

#define LOG_DEVIL_ERRORS() \
{\
	for(ILenum err = ilGetError(); err != IL_NO_ERROR; err = ilGetError()) { \
		LOG_ERROR << "DevIL_ERROR: " << iluErrorString(err) << std::endl; \
	}\
}

void Image::initDevIL() {
	static bool ilInitialized = false;;

	if(ilInitialized)
		return;
	LOG_DEBUG << "initialize DevIL" << std::endl;
	ilInit();
	iluInit();
	//ilutInit();
	//ilutRenderer(ILUT_OPENGL);
	LOG_DEVIL_ERRORS()
	ilInitialized = true;
	LOG_DEBUG << "initialized DevIL" << std::endl;
}

Image* Image::load(const std::string& fileName) {
	initDevIL();
	LOG_DEBUG << "load image: " << fileName << std::endl;

	ILuint img;
	ilGenImages(1, &img);
	ilBindImage(img);
	if (!ilLoadImage(fileName.c_str())) {
		LOG_ERROR << "can't load image: " << fileName << std::endl;
		LOG_DEVIL_ERRORS()
		ilDeleteImages(1,&img);
		throw std::runtime_error("can't load image: "+fileName);
	}

	// If the image is flipped (i.e. upside-down and mirrored, flip it the right way up!)
	ILinfo imageInfo;
	iluGetImageInfo(&imageInfo);
	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
		iluFlipImage();
	}

	return new Image(fileName, img);
}

Image::Image(const std::string& fileName, ILuint img) : img(img), fileName(fileName) {
}

Image::~Image() {
	ilDeleteImages(1, &img);
	LOG_DEVIL_ERRORS();
}

glm::uint Image::getWidth() {
	ilBindImage(img);
	return ilGetInteger(IL_IMAGE_WIDTH);
}

glm::uint Image::getHeight() {
	ilBindImage(img);
	return ilGetInteger(IL_IMAGE_HEIGHT);
}

unsigned char* Image::getData() {
	ilBindImage(img);
	return ilGetData();
}

int Image::getBytesPerPixel() {
	ilBindImage(img);
	return ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
}

int Image::getFormat() {
	ilBindImage(img);
	return ilGetInteger(IL_IMAGE_FORMAT);
}

int Image::getType() {
	ilBindImage(img);
	return ilGetInteger(IL_IMAGE_TYPE);
}



Image* loadImage(const std::string &fileName) {
	return Image::load(fileName);
}

}

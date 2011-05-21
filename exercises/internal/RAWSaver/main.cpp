/*
 * main.cpp
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#include <iostream>
#include <IL/il.h>
#include <IL/ilu.h>

#define LOG_DEVIL_ERRORS() \
{\
	for(ILenum err = ilGetError(); err != IL_NO_ERROR; err = ilGetError()) { \
		std::cerr << "DevIL_ERROR: " << iluErrorString(err) << std::endl; \
	}\
}

ILubyte* loadRawImage(int width, int height, char* path) {
	std::cout << "loading image: " << path << " with " << width << "x" << height << std::endl;
	int size = width*height*3;
	ILubyte* data = new ILubyte[size];
	FILE *file;
	file = fopen(path,"r");
	if (file != NULL) {
		fread(data,sizeof(ILubyte),size,file);
		fclose(file);
		return data;
	}
	return NULL;
}

int main(int argc, char** argv) {
	if(argc <= 4) {
		std::cerr << "use " << argv[0] << " <input> <width> <height> <output>" << std::endl;
		return 1;
	}

	char *input = argv[1];
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);

	char *output = argv[4];

	ILubyte* data = loadRawImage(width, height, input);
	if(data == NULL) {
		std::cout << "invalid file: " << input << std::endl;
		return 1;
	}

	ilInit();
	iluInit();

	ILuint image = ilGenImage();

	ilBindImage(image);

	ilTexImage(width, height, 1, 3, IL_RGB, IL_UNSIGNED_BYTE, data);

	LOG_DEVIL_ERRORS();

	ilEnable(IL_FILE_OVERWRITE);
	if(!ilSaveImage(output)) {
		std::cout << "can't save image: " << output << std::endl;
	}
	ilDeleteImage(image);

	std::cout << "saved image: " << output << std::endl;

	LOG_DEVIL_ERRORS();
	delete[] data;
	return 0;
}

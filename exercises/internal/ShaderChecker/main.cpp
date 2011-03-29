/*
 * main.cpp
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */

#include <iostream>
#include <GLee.h>
#include <GL/gl.h>		// OpenGL header
#include <GL/glut.h>	// GLUT header

#include <utils/loglevels.h>
#define LOG_LEVEL LOG_LEVEL_DEBUG

#include <oogl/GLSLShader.h>
#include <oogl/GLSLProgram.h>

#include <oogl/gl_error.h>
#include <oogl/timer.h>

int setupGLUT(int argc, char** argv) {
	glutInit(&argc, argv);

	glutInitWindowSize(1, 1);
	glutInitWindowPosition(1000,1000);
	int windowId = glutCreateWindow("Test");
	glutHideWindow();

	return windowId;
}

int main(int argc, char** argv) {
	setupGLUT(argc, argv);

	oogl::dumpGLInfos();

	if(argc <= 1) {
		LOG_ERROR << "no shader file given: use " << argv[0] << " <shader>" << std::endl;
		return 1;
	}

	switch(argc) {
	case 2: {
		std::string fragShader = std::string(argv[1]);
		try {
			std::auto_ptr<oogl::GLSLShader> s(oogl::GLSLShader::create(oogl::GLSLShader::FRAGMENT, fragShader));
			LOG_INFO << fragShader << ": success" << std::endl;
		} catch (std::exception e) {
			LOG_ERROR << fragShader << ": error occurred: " << e.what() << std::endl;
			return 1;
		}
		break;
	}
	case 3: {
		std::string vertShader = std::string(argv[1]);
		std::string fragShader = std::string(argv[2]);
		try {
			std::auto_ptr<oogl::GLSLProgram> p(oogl::GLSLProgram::create(vertShader, fragShader));
			LOG_INFO << "program(" << vertShader <<", " << fragShader << ")" << ": success" << std::endl;
		} catch (std::exception e) {
			LOG_ERROR << "program(" << vertShader <<", " << fragShader << ")" << ": error occurred: " << e.what() << std::endl;
			return 1;
		}
		break;
	}
	default: {
		std::string vertShader = std::string(argv[1]);
		std::string geomShader = std::string(argv[2]);
		std::string fragShader = std::string(argv[3]);
		try {
			std::auto_ptr<oogl::GLSLProgram> p(oogl::GLSLProgram::create(vertShader, geomShader, fragShader));
			LOG_INFO << "program(" << vertShader <<", " << geomShader <<", " << fragShader << ")" << ": success" << std::endl;
		} catch (std::exception e) {
			LOG_ERROR << "program(" << vertShader <<", " << geomShader <<", " << fragShader << ")" << ": error occurred: " << e.what() << std::endl;
			return 1;
		}
		break;
	}
	}
	return 0;
}

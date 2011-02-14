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

#include <oogl/gl_error.h>
#include <oogl/timer.h>

void init() {

}

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

	std::string shader = std::string(argv[1]);

	try {
		oogl::GLSLShader *s = new oogl::GLSLShader(oogl::GLSLShader::FRAGMENT, shader);
		delete s;
		LOG_INFO << "success" << std::endl;
	} catch (std::exception e) {
		LOG_ERROR << "error occurred: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

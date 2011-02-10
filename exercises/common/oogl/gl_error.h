/*
 * GLSLShader.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/glIncludes.h>
#include <GL/glu.h>
#include <utils/log.h>

#define LOG_GL_ERRORS() \
	{\
		for(GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) { \
			LOG_ERROR << "GL_ERROR: " << gluErrorString(err) << std::endl; \
		}\
	}


namespace oogl {
	void dumpGLInfos() {
		if(!LOG_IS_INFO_ENABLED)
			return;

		LOG_INFO << std::endl
			<< "Vendor: " << glGetString(GL_VENDOR) << std::endl
			<< "Renderer: " << glGetString(GL_RENDERER) << std::endl
			<< "Version: " << glGetString(GL_VERSION) << std::endl
			<< "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	}
}

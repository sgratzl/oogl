/*
 * gl_error.h
 *
 *  Created on: 10.02.2011
 *      Author: sam
 */
#pragma once

#include <oogl/glIncludes.h>
#if defined(__APPLE__) || defined(__APPLE_CC__)
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif
#include <utils/log.h>

#define LOG_GL_ERRORS() \
	{\
		for(GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) { \
			LOG_ERROR << "GL_ERROR: " << gluErrorString(err) << std::endl; \
		}\
	}


namespace oogl {
	void dumpGLInfos();
}


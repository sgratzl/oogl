#ifndef GL_ERROR_H
#define GL_ERROR_H

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
	void dumpGLInfos();
}

#endif

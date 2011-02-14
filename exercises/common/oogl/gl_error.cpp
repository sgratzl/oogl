/*
 * gl_error.cpp
 *
 *  Created on: 14.02.2011
 *      Author: sam
 */

#include "gl_error.h"

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

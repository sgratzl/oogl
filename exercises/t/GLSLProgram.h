/*
 * GLSLProgram.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef GLSLPROGRAM_H_
#define GLSLPROGRAM_H_

#include <GL/glew.h>
#include <utils/log.h>
#include <openglwrapper/GLSLShader.h>
#include <openglwrapper/Texture.h>
#include <map>

namespace openglwrapper {

class GLSLAttrib {
public:
	GLSLAttrib(const GLint id);

	GLSLAttrib& operator=(const int value);
	GLSLAttrib& operator=(const float value);
	GLSLAttrib& operator=(Texture* value);

private:
	GLint id;

	LOG_DECLARE_LOGGER
};

typedef boost::shared_ptr<GLSLAttrib> GLSLAttribPtr;

class GLSLProgram;
typedef boost::shared_ptr<GLSLProgram> GLSLProgramPtr;

class GLSLProgram {
public:
	GLSLProgram(const OpenGLContextPtr& context, const std::vector<GLSLShaderPtr>& shaders);
	virtual ~GLSLProgram();

	void bind() const;
	void unbind() const;

	inline GLSLAttrib& arg(const std::string& arg) { return (*this)[arg]; }

	GLSLAttrib& operator[](const std::string& arg);

	static GLSLProgramPtr create(const OpenGLContextPtr& context, const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

private:
	GLuint prog;
	std::vector<GLSLShaderPtr> shaders;
	std::map<const std::string, GLSLAttribPtr> locationCache;

	void create();
	void link();
	LOG_DECLARE_LOGGER
};

}

#endif /* GLSLPROGRAM_H_ */

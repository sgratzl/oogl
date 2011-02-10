/*
 * GLSLShader.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

#include <utils/log.h>
#include <openglwrapper/OpenGLContext.h>

namespace openglwrapper {

class GLSLShader {
public:
	enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER
	};

	GLSLShader(const OpenGLContextPtr& context, const ShaderType shaderType, const std::string& filename);
	virtual ~GLSLShader();

private:
	GLuint shader;
	friend class GLSLProgram;

	void loadFromFile(const std::string& filename);
	void load(const std::string& cod);
	LOG_DECLARE_LOGGER
};

typedef boost::shared_ptr<GLSLShader> GLSLShaderPtr;

}

#endif /* GLSLSHADER_H_ */

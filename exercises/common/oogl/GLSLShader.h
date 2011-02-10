/*
 * GLSLShader.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

#include <oogl/glIncludes.h>

#include <utils/log.h>

namespace oogl {

class GLSLShader {
public:
	enum ShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER_EXT
	};

	GLSLShader(const ShaderType shaderType, const std::string& filename);
	virtual ~GLSLShader();

private:
	GLuint shader;
	friend class GLSLProgram;

	void loadFromFile(const std::string& filename);
	void load(const std::string& cod);
};

}

#endif /* GLSLSHADER_H_ */

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

	static GLSLShader* create(const ShaderType shaderType, const std::string& filename);
	static GLuint load(const ShaderType shaderType, const std::string& filename);

	virtual ~GLSLShader();

	const char *toString(const ShaderType type) const;

private:
	ShaderType type;
	std::string filename;
	bool cleanUpOnFree;
	GLuint shader;
	friend class GLSLProgram;

	GLSLShader(const ShaderType shaderType, const std::string& filename, bool cleanUpOnFree = true);

	void loadFromFile(const std::string& filename);
	void load(const std::string& code);
};

GLuint loadShader(const std::string& filename, GLenum type);

}

#endif /* GLSLSHADER_H_ */

/*
 * GLSLProgram.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef GLSLPROGRAM_H_
#define GLSLPROGRAM_H_

#include <oogl/glIncludes.h>

#include <utils/log.h>
#include <oogl/GLSLShader.h>
#include <oogl/Texture.h>

#include <vector>
#include <map>

#include <memory>

namespace oogl {

class GLSLAttrib {
public:
	GLSLAttrib(const GLint id);

	GLSLAttrib& operator=(const int value);
	GLSLAttrib& operator=(const float value);
	GLSLAttrib& operator=(Texture* value);

private:
	GLint id;
};

typedef std::shared_ptr<GLSLAttrib> GLSLAttribPtr;

class GLSLProgram;
typedef std::shared_ptr<GLSLProgram> GLSLProgramPtr;

class GLSLProgram {
public:
	GLSLProgram(std::vector<GLSLShader> shaders);
	virtual ~GLSLProgram();

	void bind() const;
	void unbind() const;

	inline GLSLAttrib& arg(const std::string& arg) { return (*this)[arg]; }

	GLSLAttrib& operator[](const std::string& arg);

	static GLSLProgramPtr create(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	static GLSLProgramPtr create(const std::string& vertexShaderFile, const std::string& geometryShaderFile, const std::string& fragmentShaderFile);

private:
	GLuint prog;
	typedef std::vector<GLSLShader> GLSLShaders;
	GLSLShaders shaders;

	std::map<const std::string, GLSLAttribPtr> locationCache;

	void create();
	void link();
};

}

#endif /* GLSLPROGRAM_H_ */

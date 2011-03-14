/*
 * GLSLProgram.h
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#ifndef GLSLPROGRAM_H_
#define GLSLPROGRAM_H_

#include <oogl/glIncludes.h>
#include <oogl/GLSLShader.h>
#include <oogl/Texture.h>

#include <utils/log.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>

#include <memory>

namespace oogl {

class GLSLAttrib {
public:
	GLSLAttrib(const std::string name, const GLint id);

	GLSLAttrib& operator=(const int value);
	GLSLAttrib& operator=(const glm::ivec2 value);
	GLSLAttrib& operator=(const glm::ivec3 value);
	GLSLAttrib& operator=(const glm::ivec4 value);

	GLSLAttrib& operator=(const glm::uint value);
	GLSLAttrib& operator=(const glm::uvec2 value);
	GLSLAttrib& operator=(const glm::uvec3 value);
	GLSLAttrib& operator=(const glm::uvec4 value);

	GLSLAttrib& operator=(const float value);
	GLSLAttrib& operator=(const glm::vec2 value);
	GLSLAttrib& operator=(const glm::vec3 value);
	GLSLAttrib& operator=(const glm::vec4 value);

	GLSLAttrib& operator=(const bool value);
	GLSLAttrib& operator=(const glm::bvec2 value);
	GLSLAttrib& operator=(const glm::bvec3 value);
	GLSLAttrib& operator=(const glm::bvec4 value);

	GLSLAttrib& operator=(const glm::mat2 value);
	GLSLAttrib& operator=(const glm::mat3 value);
	GLSLAttrib& operator=(const glm::mat4 value);

	GLSLAttrib& operator=(Texture* value);

private:
	GLint id;
	std::string name;
};

class GLSLProgram {
public:
	GLSLProgram(std::vector<GLSLShader*> shaders);
	virtual ~GLSLProgram();

	void bind() const;
	void unbind() const;

	GLuint getId() const {
		return prog;
	}

	inline GLSLAttrib uniform(const std::string& arg) { return (*this)[arg]; }

	GLSLAttrib operator[](const std::string& arg);

	static GLSLProgram* create(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
	static GLSLProgram* create(const std::string& vertexShaderFile, const std::string& geometryShaderFile, const std::string& fragmentShaderFile);

private:
	GLuint prog;
	typedef std::vector<GLSLShader*> GLSLShaders;
	GLSLShaders shaders;

	std::map<const std::string, GLint> locationCache;

	void create();
	void link();
};

}

#endif /* GLSLPROGRAM_H_ */

/*
 * GLSLProgram.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/GLSLProgram.h>
#include <oogl/gl_error.h>

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <stdexcept>

namespace oogl {

GLSLProgram::GLSLProgram(const std::vector<GLSLShader*> shaders) :
	prog(0), shaders(shaders) {
	create();
	link();
}

GLSLProgram::~GLSLProgram() {
	LOG_DEBUG << "free program " << prog << std::endl;
	glDeleteProgram(prog);

	for(GLSLShaders::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		delete *it;
	}
	shaders.clear();

	locationCache.clear();
}

void GLSLProgram::create() {
	prog = glCreateProgram();
	LOG_GL_ERRORS();
	for (GLSLShaders::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		glAttachShader(prog, (*it)->shader);
	}
	LOG_GL_ERRORS();
}


std::string GLSLProgram::getInfoLog(GLuint programId) {
	GLint infolen;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infolen);
	if(infolen <= 0)
		return "";

	char *log = new char[infolen];
	assert(log);
	glGetProgramInfoLog(programId, infolen, NULL, log);
	std::string slog(log);
	delete [] log;

	return slog;
}

void GLSLProgram::link() {
	LOG_DEBUG << "linking program" << std::endl;
	GLint status;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);

	if (!status) {
		std::string log = "can't link program: " + getInfoLog(prog);
		LOG_ERROR << log << std::endl;
		throw std::runtime_error(log);
	} else {
		LOG_DEBUG << "linked program - no errors" << std::endl;
	}
}

void GLSLProgram::bind() const {
	LOG_DEBUG << "bind program: " << prog << std::endl;
	glUseProgram(prog);
}
void GLSLProgram::unbind() const {
	glUseProgram(0);
}

GLSLProgram* GLSLProgram::create(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
	GLSLShader* vertex = GLSLShader::create(GLSLShader::VERTEX, vertexShaderFile);
	GLSLShader* fragment = GLSLShader::create(GLSLShader::FRAGMENT, fragmentShaderFile);

	GLSLShaders shaders;
	shaders.push_back(vertex);
	shaders.push_back(fragment);

	return new GLSLProgram(shaders);
}

GLSLProgram* GLSLProgram::create(const std::string& vertexShaderFile, const std::string& geometryShaderFile, const std::string& fragmentShaderFile) {
	GLSLShader* vertex = GLSLShader::create(GLSLShader::VERTEX, vertexShaderFile);
	GLSLShader* fragment = GLSLShader::create(GLSLShader::FRAGMENT, fragmentShaderFile);
	GLSLShader* geometry = GLSLShader::create(GLSLShader::GEOMETRY, geometryShaderFile);

	GLSLShaders shaders;
	shaders.push_back(vertex);
	shaders.push_back(fragment);
	shaders.push_back(geometry);

	return new GLSLProgram(shaders);
}

GLSLAttrib GLSLProgram::operator[](const std::string& arg) {
	if (locationCache.find(arg) != locationCache.end()) {
		return GLSLAttrib(arg,locationCache[arg]);
	}
	GLint id = glGetUniformLocation(prog, arg.c_str());
	if (id < 0) {
		GLint err = glGetError();
		switch (err) {
		case 0:
			LOG_ERROR << arg << ": no error but id " << id << std::endl;
			break;
		case GL_INVALID_VALUE:
			LOG_ERROR << arg << ": no such program" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			LOG_ERROR << arg << ": invalid operation" << std::endl;
			break;
		default:
			LOG_ERROR << arg << ": unknown error" << std::endl;
			break;
		}
	}
	locationCache[arg] = id;

	return GLSLAttrib(arg, locationCache[arg]);
}

GLSLAttrib::GLSLAttrib(const std::string name, const GLint id) :
	id(id), name(name) {
}

#define OPERATOR_EQUAL(TYPE, FUNCTION) \
GLSLAttrib& GLSLAttrib::operator=(const TYPE value) {\
	if (id >= 0) {\
		FUNCTION (id, value);\
		LOG_GL_ERRORS();\
	} else {\
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;\
	}\
	return *this;\
}

#define OPERATOR_EQUAL234(TYPE, FUNCTION, SIZE) \
GLSLAttrib& GLSLAttrib::operator=(const TYPE value) {\
	if (id >= 0) {\
		FUNCTION (id, 1, glm::value_ptr(value));\
		LOG_GL_ERRORS();\
	} else {\
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;\
	}\
	return *this;\
}


OPERATOR_EQUAL(int, glUniform1i)
OPERATOR_EQUAL234(glm::ivec2, glUniform2iv, 2)
OPERATOR_EQUAL234(glm::ivec3, glUniform3iv, 3)
OPERATOR_EQUAL234(glm::ivec4, glUniform4iv, 4)

OPERATOR_EQUAL(float, glUniform1f)
OPERATOR_EQUAL234(glm::vec2, glUniform2fv, 2)
OPERATOR_EQUAL234(glm::vec3, glUniform3fv, 3)
OPERATOR_EQUAL234(glm::vec4, glUniform4fv, 4)

OPERATOR_EQUAL(glm::uint, glUniform1ui)
OPERATOR_EQUAL234(glm::uvec2, glUniform2uiv, 2)
OPERATOR_EQUAL234(glm::uvec3, glUniform3uiv, 3)
OPERATOR_EQUAL234(glm::uvec4, glUniform4uiv, 4)

#undef OPERATOR_EQUAL
#undef OPERATOR_EQUAL234


GLSLAttrib& GLSLAttrib::operator=(const bool value) {
	if (id >= 0) {
		glUniform1i(id, value?1:0);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;
	}
	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(const glm::bvec2 value) {
	if (id >= 0) {
		glUniform2i(id, value.x?1:0, value.y?1:0 );
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;
	}
	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(const glm::bvec3 value) {
	if (id >= 0) {
		glUniform3i(id, value.x?1:0, value.y?1:0, value.z?1:0);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;
	}
	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(const glm::bvec4 value) {
	if (id >= 0) {
		glUniform4i(id, value.x?1:0, value.y?1:0, value.z?1:0, value.w?1:0);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;
	}
	return *this;
}

#define OPERATOR_EQUAL_MATRIX234(TYPE, FUNCTION, SIZE) \
GLSLAttrib& GLSLAttrib::operator=(const TYPE value) {\
	if (id >= 0) {\
		FUNCTION (id, 1, false, glm::value_ptr(value));\
		LOG_GL_ERRORS();\
	} else {\
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;\
	}\
	return *this;\
}

OPERATOR_EQUAL_MATRIX234(glm::mat2, glUniformMatrix2fv, 4)
OPERATOR_EQUAL_MATRIX234(glm::mat3, glUniformMatrix3fv, 9)
OPERATOR_EQUAL_MATRIX234(glm::mat4, glUniformMatrix4fv, 16)

#undef OPERATOR_EQUAL_MATRIX234

GLSLAttrib& GLSLAttrib::operator=(Texture* value) {
	if (id >= 0 && value->isBound()) {
		glUniform1i(id, value->bindedTexture);
		LOG_GL_ERRORS();
	} else if(!value->isBound()){
		LOG_ERROR << "texture(" << value->textureId << ") is not bound" << std::endl;
	} else {
		LOG_WARN << "skip setting invalid parameter: " << name << std::endl;
	}
	return *this;
}

#define GENERIC_SET(TYPE)\
	void GLSLAttrib::set(const TYPE value) {\
		(*this) = value; \
	}

GENERIC_SET(int)
GENERIC_SET(glm::ivec2)
GENERIC_SET(glm::ivec3)
GENERIC_SET(glm::ivec4)

GENERIC_SET(glm::uint)
GENERIC_SET(glm::uvec2)
GENERIC_SET(glm::uvec3)
GENERIC_SET(glm::uvec4)

GENERIC_SET(float)
GENERIC_SET(glm::vec2)
GENERIC_SET(glm::vec3)
GENERIC_SET(glm::vec4)

GENERIC_SET(bool)
GENERIC_SET(glm::bvec2)
GENERIC_SET(glm::bvec3)
GENERIC_SET(glm::bvec4)

GENERIC_SET(glm::mat2)
GENERIC_SET(glm::mat3)
GENERIC_SET(glm::mat4)

void GLSLAttrib::set(Texture* value) {\
	(*this) = value; \
}

#undef GENERIC_SET
}

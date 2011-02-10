/*
 * GLSLProgram.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <oogl/GLSLProgram.h>
#include <oogl/gl_error.h>

#include <sstream>

namespace oogl {

GLSLProgram::GLSLProgram(const std::vector<GLSLShader> shaders): prog(0), shaders(shaders) {
	create();
	link();
}

GLSLProgram::~GLSLProgram() {
	glDeleteProgram(prog);
	locationCache.clear();
}

void GLSLProgram::create() {
	prog = glCreateProgram();
	LOG_GL_ERRORS();
	for(GLSLShaders::iterator it = shaders.begin(); it != shaders.end(); ++it) {
		glAttachShader(prog, it->shader);
	}
	LOG_GL_ERRORS();
}

void GLSLProgram::link() {
	LOG_DEBUG << "linking program" << std::endl;
	GLint status;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(!status) {
		GLint len;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		char *log = new char[len];
		glGetProgramInfoLog(prog, len, NULL, log);

		LOG_ERROR << "can't link program: " << log << std::endl;
		std::ostringstream error_string;
		error_string << "can't link program: " << log;
		delete [] log;
		throw std::runtime_error(error_string.str());
	} else {
		LOG_DEBUG << "linked program - no errors" << std::endl;
	}
}

void GLSLProgram::bind() const {
	LOG_DEBUG << "bind program" << std::endl;
	glUseProgram(prog);
}
void GLSLProgram::unbind() const {
	glUseProgram(0);
}


GLSLProgramPtr GLSLProgram::create(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
	GLSLShader vertex(GLSLShader::VERTEX, vertexShaderFile);
	GLSLShader fragment(GLSLShader::FRAGMENT, fragmentShaderFile);

	GLSLShaders shaders;
	shaders.push_back(vertex);
	shaders.push_back(fragment);

	return GLSLProgramPtr(new GLSLProgram(shaders));
}

GLSLProgramPtr GLSLProgram::create(const std::string& vertexShaderFile, const std::string& geometryShaderFile, const std::string& fragmentShaderFile) {
	GLSLShader vertex(GLSLShader::VERTEX, vertexShaderFile);
	GLSLShader fragment(GLSLShader::FRAGMENT, fragmentShaderFile);
	GLSLShader geometry(GLSLShader::GEOMETRY, geometryShaderFile);

	GLSLShaders shaders;
	shaders.push_back(vertex);
	shaders.push_back(fragment);
	shaders.push_back(geometry);

	return GLSLProgramPtr(new GLSLProgram(shaders));
}
static GLSLProgramPtr create();

GLSLAttrib& GLSLProgram::operator[](const std::string& arg) {
	if(locationCache.find(arg) != locationCache.end()) {
		return *locationCache[arg];
	}
	GLint id = glGetUniformLocation(prog, arg.c_str());
	if(id < 0) {
		GLint err = glGetError();
		switch(err) {
			case 0: break;
			case GL_INVALID_VALUE:
				LOG_ERROR << arg.c_str() << ": no such program" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				LOG_ERROR << arg.c_str() << ": invalid operation" << std::endl;
				break;
			default:
				LOG_ERROR << arg.c_str() << ": unknown error" << std::endl;
				break;
		}
	}
	GLSLAttribPtr att(new GLSLAttrib(id));
	locationCache[arg] = att;

	return *locationCache[arg].get();
}


GLSLAttrib::GLSLAttrib(const GLint id): id(id) {}

GLSLAttrib& GLSLAttrib::operator=(const int value) {
	if(id >= 0) {
		glUniform1i(id, value);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter" << std::endl;
	}

	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(const float value) {
	if(id >= 0) {
		glUniform1f(id, value);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter" << std::endl;
	}
	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(Texture* value) {
	if(id >= 0) {
		glUniform1i(id, value->textureId);
		LOG_GL_ERRORS();
	} else {
		LOG_WARN << "skip setting invalid parameter" << std::endl;
	}
	return *this;
}

}

/*
 * GLSLProgram.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <openglwrapper/GLSLProgram.h>
#include <openglwrapper/gl_error.h>
#include <boost/foreach.hpp>

namespace openglwrapper {

LOG_DEFINE_LOGGER(GLSLProgram, openglwrapper.GLSLProgram)
		LOG_DEFINE_LOGGER(GLSLAttrib, openglwrapper.GLSLAttrib)

GLSLProgram::GLSLProgram(const OpenGLContextPtr& context, const std::vector<GLSLShaderPtr>& shaders): prog(0), shaders(shaders) {
	create();
	link();
}

GLSLProgram::~GLSLProgram() {
	glDeleteProgram(prog);
	locationCache.clear();
}

void GLSLProgram::create() {
	prog = glCreateProgram();
	gl_check_error();
	BOOST_FOREACH(GLSLShaderPtr s, shaders) {
		glAttachShader(prog, s->shader);
	}
	gl_check_error();
}

void GLSLProgram::link() {
	LOG_TRACE("linking program");
	GLint status;
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);
	if(!status) {
		GLint len;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		char *log = new char[len];
		glGetProgramInfoLog(prog, len, NULL, log);

		LOG_ERROR("can't link program: " << log);
		std::string error_string = std::string("can't link program: ") + std::string(log);
		delete [] log;
		throw std::runtime_error(error_string);
	} else {
		LOG_DEBUG("linked program - no errors");
	}
}

void GLSLProgram::bind() const {
	LOG_TRACE("bind program");
	glUseProgram(prog);
}
void GLSLProgram::unbind() const {
	glUseProgram(0);
}


GLSLProgramPtr GLSLProgram::create(const OpenGLContextPtr& context, const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
	GLSLShaderPtr vertex(new GLSLShader(context, GLSLShader::VERTEX, vertexShaderFile));
	GLSLShaderPtr fragment(new GLSLShader(context, GLSLShader::FRAGMENT, fragmentShaderFile));

	std::vector<GLSLShaderPtr> shaders;
	shaders.push_back(vertex);
	shaders.push_back(fragment);

	return GLSLProgramPtr(new GLSLProgram(context, shaders));
}



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
				LOG_ERROR("no such program");
				break;
			case GL_INVALID_OPERATION:
				LOG_ERROR("invalid operation");
				break;
			default:
				LOG_ERROR("unknown error");
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
	} else {
		LOG_WARN("skip setting invalid parameter");
	}

	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(const float value) {
	if(id >= 0) {
		glUniform1f(id, value);
	} else {
		LOG_WARN("skip setting invalid parameter");
	}
	return *this;
}
GLSLAttrib& GLSLAttrib::operator=(Texture* value) {
	if(id >= 0) {
		glUniform1i(id, value->getId());
	} else {
		LOG_WARN("skip setting invalid parameter");
	}
	return *this;
}

}

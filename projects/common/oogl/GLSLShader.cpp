/*
 * GLSLShader.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <cassert>
#include <stdexcept>
#include <oogl/GLSLShader.h>
#include <oogl/gl_error.h>
#include <iostream>
#include <fstream>
#include <string>

namespace oogl {

GLSLShader* GLSLShader::create(const ShaderType shaderType, const std::string& filename) {
	return new GLSLShader(shaderType, filename);
}

GLuint GLSLShader::load(const ShaderType shaderType, const std::string& filename) {
	GLSLShader s(shaderType, filename, false);
	return s.shader;
}

GLSLShader::GLSLShader(const ShaderType type, const std::string& filename, bool cleanUpOnFree): type(type), filename(filename), cleanUpOnFree(cleanUpOnFree) {
	shader = glCreateShader(type);
	if(!shader) {
		LOG_ERROR << "failed creating shader of type: " << toString(type) << std::endl;
		throw std::runtime_error("failed creating shader");
	}
	loadFromFile(filename);
}

GLSLShader::~GLSLShader() {
	if(cleanUpOnFree) {
		LOG_DEBUG << "free shader " << filename << std::endl;
		glDeleteShader(shader);
	}
}

const char *GLSLShader::toString(const ShaderType type) const {
	switch(type) {
	case VERTEX: return "Vertex";
	case FRAGMENT: return "Fragment";
	case GEOMETRY: return "Geometry";
	default: return "Unknown";
	}
}

std::string GLSLShader::getInfoLog(GLuint shaderId) {
	GLint infolen;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infolen);
	if(infolen <= 0)
		return "";

	char *log = new char[infolen];
	assert(log);
	glGetShaderInfoLog(shaderId, infolen, NULL, log);
	std::string slog(log);
	delete [] log;

	return slog;
}

void GLSLShader::load(const std::string& code) {
	GLint status;
	const GLchar* tmp[2] = {code.c_str(), 0};
	glShaderSource(shader, 1, tmp, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	std::string log = getInfoLog(shader);

	if(!status) {
		LOG_ERROR << filename << "(" << toString(type) << "): can't compile shader: \n" << log << std::endl;
		throw std::runtime_error("can't compile shader: " + log);
	} else {
		/* maybe there are warnings? */
		if(log == "" || log == "No errors.") {
			LOG_DEBUG << filename << "(" << toString(type) << "): compiled shader: -> no warnings" << std::endl;
		} else {
			LOG_WARN << filename << "(" << toString(type) << "): shader warnings:\n" << log << std::endl;
		}
	}
}

void GLSLShader::loadFromFile(const std::string& filename) {
	LOG_DEBUG << "loading file: " << filename << std::endl;
	std::ifstream in(filename.c_str());
	if(!in) {
		LOG_ERROR << "invalid file: " << filename << std::endl;
		return;
	}
	load(std::string(std::istreambuf_iterator<char>(in),std::istreambuf_iterator<char>()));
	in.close();
}

GLuint loadShader(const std::string& filename, GLenum type) {
	return GLSLShader::load((GLSLShader::ShaderType)type, filename);
}

}

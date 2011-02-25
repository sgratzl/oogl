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

GLSLShader::GLSLShader(const ShaderType type, const std::string& filename): type(type), filename(filename) {
	shader = glCreateShader(type);
	if(!shader) {
		LOG_ERROR << "failed creating shader of type: " << toString(type) << std::endl;
		throw std::runtime_error("failed creating shader");
	}
	loadFromFile(filename);
}

GLSLShader::~GLSLShader() {
	LOG_DEBUG << "free shader " << filename << std::endl;
	glDeleteShader(shader);
}

const char *GLSLShader::toString(const ShaderType type) const {
	switch(type) {
	case VERTEX: return "Vertex";
	case FRAGMENT: return "Fragment";
	case GEOMETRY: return "Geometry";
	default: return "Unknown";
	}
}

void GLSLShader::load(const std::string& code) {
	GLint status;
	const GLchar* tmp[2] = {code.c_str(), 0};
	glShaderSource(shader, 1, tmp, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if(!status) {
		GLint infolen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
		char *log = new char[infolen];
		assert(log);
		glGetShaderInfoLog(shader, infolen, NULL, log);
		LOG_ERROR << filename << "(" << toString(type) << "): can't compile shader: \n" << log << std::endl;
		std::string error_str = "can't compile shader: " + std::string(log);
		delete [] log;
		throw std::runtime_error(error_str);
	} else {
		/* maybe there are warnings? */
		GLint infolen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
		if(!infolen) {
			LOG_DEBUG << filename << "(" << toString(type) << "): compiled shader: -> no warnings" << std::endl;
			return;
		}
		char *log = new char[infolen];
		assert(log);
		glGetShaderInfoLog(shader, infolen, NULL, log);
		std::string logs(log);
		if(logs != "No errors.") {
			LOG_WARN << filename << "(" << toString(type) << "): shader warnings:\n" << log << std::endl;
		}
		delete [] log;
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

}

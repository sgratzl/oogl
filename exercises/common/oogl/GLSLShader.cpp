/*
 * GLSLShader.cpp
 *
 *  Created on: 12.07.2010
 *      Author: sam
 */

#include <cassert>
#include <oogl/GLSLShader.h>
#include <oogl/gl_error.h>
#include <iostream>
#include <fstream>
#include <string>

namespace oogl {

GLSLShader::GLSLShader(const ShaderType type, const std::string& filename) {
	shader = glCreateShader(type);
	if(!shader) {
		LOG_ERROR << "failed creating shader of type: " << glGetString(type) << std::endl;
		throw std::runtime_error("failed creating shader");
	}
	loadFromFile(filename);
}

GLSLShader::~GLSLShader() {
	glDeleteShader(shader);
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
		LOG_ERROR << "can't compile shader: \n" << log << std::endl;
		std::string error_str = "can't compile shader: " + std::string(log);
		delete [] log;
		throw std::runtime_error(error_str);
	} else {
		/* maybe there are warnings? */
		GLint infolen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infolen);
		if(!infolen) {
			LOG_DEBUG << "compiled shader: -> no warnings" << std::endl;
			return;
		}
		char *log = new char[infolen];
		assert(log);
		glGetShaderInfoLog(shader, infolen, NULL, log);
		std::string logs(log);
		if(logs != "No errors.") {
			LOG_WARN << "shader warnings:\n" << log << std::endl;
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

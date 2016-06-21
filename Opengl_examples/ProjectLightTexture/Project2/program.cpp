#define _CRT_SECURE_NO_WARNINGS
#include "program.h"

#include <iostream>
#include <cstring>
#include <fstream>

Program::Program(const std::string& frag, const std::string& vert)
{
	GLuint fs = initShaders(GL_FRAGMENT_SHADER, frag.c_str());
	GLuint vs = initShaders(GL_VERTEX_SHADER, vert.c_str());

	_program = glCreateProgram();

	glAttachShader(_program, vs);
	glAttachShader(_program, fs);

	glLinkProgram(_program);

	glDeleteShader(fs);
	glDeleteShader(vs);
}

Program::~Program()
{
	glUseProgram(0);
	glDeleteProgram(_program);
}

void Program::use()
{
	glUseProgram(_program);
}

void Program::shaderErrors(const GLint shader)
{
	GLint length;
	GLchar * log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	log = new GLchar[length + 1];
	glGetShaderInfoLog(shader, length, &length, log);
	std::cout << "Compile Error, Log Below\n" << log << "\n";
	delete[] log;
}

std::string Program::textFileRead(const char * filename)
{
	std::string str, ret = "";
	std::ifstream in;
	in.open(filename);
	if (in.is_open()) {
		std::getline(in, str);
		while (in) {
			ret += str + "\n";
			getline(in, str);
		}
		return ret;
	}
	else {
		std::cerr << "Unable to Open File " << filename << "\n";
		throw 2;
	}
}

GLuint Program::initShaders(GLenum type, const char * filename)
{
	GLuint shader = glCreateShader(type);
	GLint compiled;
	std::string str = textFileRead(filename);
	GLchar * cstr = new GLchar[str.size() + 1];
	const GLchar * cstr2 = cstr;
	strcpy(cstr, str.c_str());
	glShaderSource(shader, 1, &cstr2, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		shaderErrors(shader);
		throw 3;
	}
	return shader;
}

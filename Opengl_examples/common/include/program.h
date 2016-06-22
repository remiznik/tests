#ifndef __INCLUDEPROGRAM
#define __INCLUDEPROGRAM

#include <string>
#include <gl/glew.h>

class Program
{
public:
	Program(const std::string& frag, const std::string& vert);
	~Program();

	void use();
	GLuint id() const { return _program; }

private:
	GLuint initShaders(GLenum type, const char *filename);
	void shaderErrors(const GLint shader);
	std::string textFileRead(const char * filename);

private:
	GLuint _program;
};

#endif
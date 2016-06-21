#ifndef __INCLUDETEXTURE
#define __INCLUDETEXTURE

#include <GL/glew.h>

#include <string>

class Texture
{
public:
	Texture(const std::string& name);
	~Texture();

	GLuint id() const { return _texture; }
private:
	GLuint _texture;
};

#endif
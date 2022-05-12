#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture(const std::string& fileName);

	void Bind(unsigned int unit); // bind upto 32 textures

	~Texture();

protected:
private:

	GLuint textureHandler;
};


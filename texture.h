#pragma once

#include "glad/glad.h"
#include <string>

class Texture
{
public:
	Texture(const std::string& imagePath);
	~Texture();

	void bind(unsigned int unit = 0) const;

	unsigned int getID() const { return myTexture; }

	bool bIsValid;

private:
	unsigned int myTexture;
	GLfloat filtering;

	int myWidth, myHeight, myNrChannels;
	unsigned char* myData;

};
#include "texture.h"
#include "utilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(int width, int height, int nrCHannels, std::string imagePath) :
	myTexture(0),
	filtering(0.0f),
	myWidth(width),
	myHeight(height),
	myNrChannels(nrCHannels),
	myData(nullptr),
	bIsValid(true)
{
	stbi_set_flip_vertically_on_load(true);
	myData = stbi_load(imagePath.c_str(), &myWidth, &myHeight, &myNrChannels, 0);
	if (!myData)
	{
		utilities::log("Failed to load texture : " + std::string(imagePath));
		bIsValid = false;
		return;
	}

	glGenTextures(1, &myTexture);
	glBindTexture(GL_TEXTURE_2D, myTexture);

	//Arbitrary wrapping/filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLenum format = (myNrChannels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, format, myWidth, myHeight, 0, format, GL_UNSIGNED_BYTE, myData);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::bind(unsigned int unit) const
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, myTexture);
}

Texture::~Texture()
{
	glDeleteTextures(1, &myTexture);
	stbi_image_free(myData);
}

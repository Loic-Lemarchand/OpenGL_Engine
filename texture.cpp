#include "texture.h"
#include "utilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& imagePath) :
	myTexture(0),
	filtering(0.0f),
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

	GLenum internalFormat;
	switch (myNrChannels)
	{
	case 1:
		format = GL_RED;
		internalFormat = GL_RED;
		break;
	case 2:
		format = GL_RG;
		internalFormat = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		internalFormat = GL_RGB;
		break;
	case 4:
		format = GL_RGBA;
		internalFormat = GL_RGBA;
		break;
	default:
		utilities::log("Unsupported number of channels: " + std::to_string(myNrChannels) + " for texture: " + imagePath);
		stbi_image_free(myData);
		myData = nullptr;
		bIsValid = false;
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

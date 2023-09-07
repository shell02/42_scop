#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::Texture(std::string texturePath, bool flip) : path(texturePath)
{
	if (flip)
		stbi_set_flip_vertically_on_load(true);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// std::cout << "My id: " << id << " My path:" << texturePath << std::endl; 

	data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 1)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		gl_log_err("ERROR: **TEXTURE**: Failed to load image at %s\n", texturePath);
	}
	stbi_image_free(data);

	if (flip)
		stbi_set_flip_vertically_on_load(false);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::Texture(Texture const &copy)
{
	*this = copy;
}

Texture& Texture::operator=(Texture const& copy)
{
	this->data = copy.data;
	this->height = copy.height;
	this->width = copy.width;
	this->id = copy.id;
	this->nrChannels = copy.nrChannels;
	this->path = copy.path;
	return *this;
}

void Texture::bind(void) const
{
	glBindTexture(GL_TEXTURE_2D, id);
}


unsigned int Texture::getID() const
{
	return id;
}
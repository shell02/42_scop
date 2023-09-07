#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

bool gl_log_err(const char *message, ...);

class Texture
{
	public:
		Texture(std::string texturePath, bool flip = false);
		Texture(Texture const &copy);

		Texture& operator=(Texture const& copy);

		void	bind(void) const;

		unsigned int getID() const;


	private:

		std::string path;

		unsigned int id;
		int	width;
		int	height;
		int nrChannels;
		unsigned char* data;

};

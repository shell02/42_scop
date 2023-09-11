#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Shader
{
	public:
		Shader(const char* vertexPath, const char* fragmentPath);

		unsigned int getId() const;

		void use() const;

		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void set3Float(const std::string &name, float value1, float value2, float value3) const;
		void set4Float(const std::string &name, float value1, float value2, float value3, float value4) const;
		void setMat4(const std::string &name, float value[16], bool flip = true) const;
		

		void reloadShaderFiles(const char* vertexPath, const char* fragmentPath);

		int getErr() const;

	private:
		unsigned int ID;

		int err;

};
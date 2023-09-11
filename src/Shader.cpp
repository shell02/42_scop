#include "Shader.hpp"

bool gl_log_err(const char *message, ...);

std::string LoadShaderString(const char* fileName)
{
	std::fstream file;
	std::stringstream shaderText;

	file.open(fileName);
	if (file.is_open())
	{
		shaderText << file.rdbuf();
		file.close();
	}
	else
	{
		gl_log_err("LoadShaderString: unable to open file %s\n", fileName);
	}
	return shaderText.str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) : err(0)
{
	reloadShaderFiles(vertexPath, fragmentPath);
}

unsigned int Shader::getId() const
{
	return ID;
}

void Shader::use() const
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set3Float(const std::string &name, float value1, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3);
}

void Shader::set4Float(const std::string &name, float value1, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value1, value2, value3, value4);
}

void Shader::setMat4(const std::string &name, float value[16], bool flip) const
{
	if (flip)
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_TRUE, value);
	else
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::reloadShaderFiles(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexShaderCode = LoadShaderString(vertexPath);
	std::string fragmentShaderCode = LoadShaderString(fragmentPath);

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

	int success;
	char infoLog[512];
	const char* src = vertexShaderCode.c_str();

	glShaderSource(vertex, 1, &src, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		gl_log_err("ERROR: **VERTEX_SHADER** : %s\n", infoLog);
		err = 1;
	}

	src = fragmentShaderCode.c_str();
	glShaderSource(fragment, 1, &src, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		gl_log_err("ERROR: **FRAGMENT_SHADER** : %s\n", infoLog);
		err = 1;
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		gl_log_err("ERROR: **PROGRAM_LINKING** : %s\n", infoLog);
		err = 1;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

int Shader::getErr() const
{
	return err;
}

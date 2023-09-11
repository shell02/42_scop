#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"

class Light {
	public:
		Light(Vector3 position = Vector3(0.0f, 2.0f, 2.0f));
		~Light();

		void clear();
		void use() const;
		void draw(Matrix4 view, Matrix4 projection) const;

		Vector3 getLightPos() const;
		Vector3 getLightColor() const;
		Vector3 getLightAmb() const;
		Vector3 getLightDiff() const;

		void setLightPos(Vector3 pos);
		void setLightColor(Vector3 col);
		void setLightAmb(Vector3 amb);
		void setLightDiff(Vector3 diff);

		int getErr() const;

	private:
		unsigned int lightVAO;
		unsigned int lightVBO;

		Shader lightProgram;

		Vector3	ambient;
		Vector3	diffuse;
		Vector3 color;
		Vector3 position;

		int err;
};
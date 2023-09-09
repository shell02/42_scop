#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <ctime>
#include <map>

#include "Vector.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "MTL.hpp"

struct Vertex {
	Vector3	Position;
	Vector3	Normal;
	float	TexCoords[2];
};

class Mesh {
	public:

		std::vector<Vertex>	vertices;
		std::map<int, unsigned int>	indices;
		std::vector<MTL>	textures;

		Mesh();
		~Mesh();
		Mesh(std::vector<Vertex> vertices, std::map<int, unsigned int> indices, std::vector<MTL> textures);

		void draw(Shader const &program);
		void bindVAO() const;
		void bindVBO() const;
		void bindEBO() const;

		void normalizeV(float maxX, float maxY, float maxZ, float minX, float minY, float minZ);
		void center(float centerX, float centerY, float centerZ);

		void setIsTextured(bool isTextured);
		void setRotation(int axis);
		void setMove(int axis, float move);


	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		Matrix4 scaleMat;
		Matrix4 transMat;
		Matrix4 rotateMat;
		Matrix4 moveMat;
		Vector3 rotationAxis;

		std::vector<Vector3> colorsA;
		std::vector<Vector3> colorsD;

		float moveX;
		float moveY;
		float moveZ;

		int axis;

		void setupMesh();
};
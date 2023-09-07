#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
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
		std::map<int, std::vector<unsigned int>>	indices;
		std::vector<MTL>	textures;

		Mesh();
		Mesh(std::vector<Vertex> vertices, std::map<int, std::vector<unsigned int>> indices, std::vector<MTL> textures);

		void draw(Shader const &program);
		void bindVAO() const;
		void bindVBO() const;
		void bindEBO() const;


	private:
		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;

		void setupMesh();
};
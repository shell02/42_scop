#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <map>

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Light.hpp"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "MTL.hpp"

class Model {
	public:
		Model(std::string filename);

		void draw(Shader const &program);

		int getErr() const;

		std::vector<Mesh> getObjects() const;

	private:
		int err;
		std::string dirPath;

		std::vector<Mesh>	objects;

		int checkFile(std::string filename);
		std::string getOpt(std::string line);
		std::map<std::string, MTL> parseMTL(std::string line);
		Vector3 parsePos(std::string line);
		Vector3 parseNorm(std::string line);
		Vector3 parseTexC(std::string line);
		void parseToIndice(std::string line, std::vector<unsigned int>& ind);
		std::vector<Vertex> fillVertices(std::vector<Vector3> positions, std::vector<Vector3> normals, std::vector<float[2]> coords, std::vector<unsigned int> faces);

};
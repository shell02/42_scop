#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
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
		~Model();

		void draw(Shader const &program);
		void setTexture(bool isTextured);
		void setRotation(int axis);
		void setMove(int axis, float move);

		int getErr() const;

		std::vector<Mesh> getObjects() const;

	private:
		int err;
		float minX;
		float minY;
		float maxX;
		float maxY;
		float maxZ;
		float minZ;
		std::string dirPath;

		std::vector<Mesh>	objects;

		int checkFile(std::string filename);
		std::string getOpt(std::string line);
		std::string rmOpt(std::string line);
		std::string getArg(std::string line);
		std::map<std::string, MTL> parseMTL(std::string line);
		Vector3 parseVec3(std::string line);
		Vector3 parseTexC(std::string line);
		Vector3 getFace(std::string line);
		void parseToIndice(std::string line, std::vector<Vertex>& vertices, std::vector<unsigned int>& faces, std::vector<Vector3> &positions, std::vector<Vector3> &normals, std::vector<Vector3> &coords);

};
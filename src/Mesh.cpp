#include "Mesh.hpp"

Mesh::Mesh()
{
	return ;	
}


Mesh::Mesh(std::vector<Vertex> vertices, std::map<int, unsigned int> indices, std::vector<MTL> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	rotationAxis = Vector3(1.0f, 0.0f, 0.0f);

	setupMesh();	
}

void Mesh::draw(Shader const &program)
{
	float transMatrix[16];
	Matrix4 model = model.model(scaleMat, model.rotate(glfwGetTime() * 90.0f, rotationAxis), transMat);
	int colorIDX = 0;
	int prevSize = 0;
	int idx = 0;

	program.use();
	bindVAO();

	for (std::map<int, unsigned int>::iterator it = indices.begin(); it != indices.end(); it++) {
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		// glBufferData(GL_ELEMENT_ARRAY_BUFFER, it->second * sizeof(unsigned int), &(it->second)[0], GL_STATIC_DRAW);

		model.getMatrix(transMatrix);
		program.setMat4("model", transMatrix);
		program.setFloat("material.shininess", textures[idx].getShiny());
		program.setFloat("material.transparency", textures[idx].getTr());
		program.setBool("isTextured", textures[idx].getIsTextured());


		if (textures[idx].getIsTextured() == false) {

			// program.set3Float("material.ambient", textures[idx].getAmbient()[0], textures[idx].getAmbient()[1], textures[idx].getAmbient()[2]);
			// program.set3Float("material.diff", textures[idx].getDiff()[0], textures[idx].getDiff()[1], textures[idx].getDiff()[2]);
			program.set3Float("material.spec", textures[idx].getSpec()[0], textures[idx].getSpec()[1], textures[idx].getSpec()[2]);
			// glDrawElements(GL_TRIANGLES, it->second, GL_UNSIGNED_INT, 0);
		
			// std::cout << "drawing: " << it->second / 3 << "times." << std::endl;
			for (size_t i = 0; i * 3 < it->second; i++) {
				program.set3Float("material.ambient", colorsA[i + colorIDX][0], colorsA[i + colorIDX][1], colorsA[i + colorIDX][2]);
				program.set3Float("material.diff", colorsD[i + colorIDX][0], colorsD[i + colorIDX][1], colorsD[i + colorIDX][2]);
				// std::cout << "Color[0]: " << colors[i + colorIDX][0] << " Color[1]: " << colors[i + colorIDX][1] << " Color[2]: " << colors[i + colorIDX][2] << std::endl;
				glDrawArrays(GL_TRIANGLES, i * 3 + prevSize, 3);
			}
			colorIDX += it->second / 3;
		}
		else {
			textures[idx].bindDiff();
			textures[idx].bindSpec();
			program.setInt("material.diffuse", 0);
			program.setInt("material.specular", 1);
			glDrawArrays(GL_TRIANGLES, prevSize, it->second);

			// std::cout << texture.getSpecular().getID() << "/" << texture.getDiffuse().getID() << std::endl;
		}
		idx++;
		prevSize = it->second;
	}

	// if (indices.size() == 0) {

	// 	model.getMatrix(transMatrix);
	// 	program.setMat4("model", transMatrix);
	// 	program.setFloat("material.shininess", textures[0].getShiny());
	// 	// std::cout << "Shiny: " << texture.getShiny() << std::endl;
	// 	program.setFloat("material.transparency", textures[0].getTr());
	// 	// std::cout << "trans: " << texture.getTr() << std::endl;
	// 	program.setBool("isTextured", textures[0].getIsTextured());
	// 	// std::cout << "IsText: " << texture.getIsTextured() << std::endl;

	// 	if (textures[0].getIsTextured() == false) {

	// 		program.set3Float("material.ambient", textures[0].getAmbient()[0], textures[0].getAmbient()[1], textures[0].getAmbient()[2]);
	// 		program.set3Float("material.diff", textures[0].getDiff()[0], textures[0].getDiff()[1], textures[0].getDiff()[2]);
	// 		program.set3Float("material.spec", textures[0].getSpec()[0], textures[0].getSpec()[1], textures[0].getSpec()[2]);
	// 		// glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	// 	}
	// 	else {
	// 		textures[0].bindDiff();
	// 		textures[0].bindSpec();
	// 		program.setInt("material.diffuse", 0);
	// 		program.setInt("material.specular", 1);
	// 		// std::cout << texture.getSpecular().getID() << "/" << texture.getDiffuse().getID() << std::endl;
	// 	}
	// 	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	// }
	glBindVertexArray(0);
}

void Mesh::bindVAO() const
{
	glBindVertexArray(VAO);
}

void Mesh::bindVBO() const
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Mesh::bindEBO() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void Mesh::normalizeV(float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
	float scaleX = 1.4f / (maxX - minX);
	float scaleY = 1.4f / (maxY - minY);
	float scaleZ = 1.4f / (maxZ - minZ);
	float finalScale = scaleX;

	// if (maxX <= 0.7f && minX >= -0.7f)
	// 	scaleX = 1.0f;
	// if (maxY <= 0.7f && minY >= -0.7f)
	// 	scaleY = 1.0f;
	// if (maxZ <= 0.7f && minZ >= -0.7f)
	// 	scaleZ = 1.0f;

	if (finalScale > scaleY)
		finalScale = scaleY;
	if (finalScale > scaleZ)
		finalScale = scaleZ;


	scaleMat = scaleMat.scale(Vector3(finalScale, finalScale, finalScale));
}

void Mesh::center(float centerX, float centerY, float centerZ)
{
	transMat = transMat.translation(Vector3(centerX, centerY, centerZ));
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	//vertex texCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int idx = 0;
	srand (static_cast <unsigned> (time(0)));
	for (std::map<int, unsigned int>::iterator it = indices.begin(); it != indices.end(); it++) {

		if (textures[idx].getIsTextured() == false) {
		
			for (size_t i = 0; i * 3 < it->second; i++) {
				float random =  -0.4f + static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 0.8f);

				colorsA.push_back(Vector3(textures[idx].getAmbient()[0] + random, textures[idx].getAmbient()[1] + random, textures[idx].getAmbient()[2] + random));
				colorsD.push_back(Vector3(textures[idx].getDiff()[0] + random, textures[idx].getDiff()[1] + random, textures[idx].getDiff()[2] + random));
				// std::cout << "Color[0]: " << textures[idx].getAmbient()[0] << " Color[1]: " << textures[idx].getAmbient()[1] << " Color[2]: " << textures[idx].getAmbient()[2] << std::endl;
			}
		}
		idx++;
	}
	// int prevSize = 0;
	// int i = 0;
	// std::cout << "Num Colors: " << colors.size() << std::endl;
	// std::cout << "Color[0]: " << colors[i + prevSize][0] << " Color[1]: " << colors[i + prevSize][1] << " Color[2]: " << colors[i + prevSize][2] << std::endl;
	

	// if (indices.size() == 0) {

	// 	if (textures[0].getIsTextured() == false) {

	// 		for (size_t i = 0; i * 3 < vertices.size(); i++) {
	// 			float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f;
	// 			colors.push_back(Vector3(textures[idx].getAmbient()[0] + random, textures[idx].getAmbient()[1] + random, textures[idx].getAmbient()[2] + random));
	// 		}
	// 	}

	// }
}



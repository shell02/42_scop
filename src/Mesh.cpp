#include "Mesh.hpp"

Mesh::Mesh()
{
	rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
	moveMat = Matrix4(1.0f);
	rotateMat = Matrix4(1.0f);

	moveX = 0.0f;
	moveY = 0.0f;
	moveZ = 0.0f;
	axis = 1;
	return ;
}


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MTL> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	rotationAxis = Vector3(0.0f, 1.0f, 0.0f);
	moveMat = Matrix4(1.0f);
	rotateMat = Matrix4(1.0f);

	moveX = 0.0f;
	moveY = 0.0f;
	moveZ = 0.0f;
	axis = 1;

	// setupMesh();
}

Mesh::~Mesh()
{
	// glDeleteBuffers(1, &VBO);
	// glDeleteVertexArrays(1, &VAO);
	colorsA.clear();
	colorsD.clear();
	indices.clear();
	vertices.clear();
	textures.clear();
}

void Mesh::draw(Shader const &program)
{
	float transMatrix[16];
	Matrix4 model(1.0f);
	setRotation(this->axis);
	// model = moveMat.mulMatrix(model.model(scaleMat, rotateMat, transMat));
	model = moveMat.mulMatrix(model.model(rotateMat));
	int colorIDX = 0;
	int prevSize = 0;
	int idx = 0;

	program.use();
	bindVAO();

	for (std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); it++) {

		model.getMatrix(transMatrix);
		program.setMat4("model", transMatrix);
		program.setFloat("material.shininess", textures[idx].getShiny());
		program.setFloat("material.transparency", textures[idx].getTr());
		program.setBool("isTextured", textures[idx].getIsTextured());
		// textures[idx].bindDiff();
		// textures[idx].bindSpec();
		// program.setInt("material.diffuse", 0);
		// program.setInt("material.specular", 1);
		// std::cout << "Number of vertices: " << *it << std::endl;


		if (textures[idx].getIsTextured() == false) {

			// program.set3Float("material.ambient", textures[idx].getAmbient()[0], textures[idx].getAmbient()[1], textures[idx].getAmbient()[2]);
			// program.set3Float("material.diff", textures[idx].getDiff()[0], textures[idx].getDiff()[1], textures[idx].getDiff()[2]);
			program.set3Float("material.spec", textures[idx].getSpec()[0], textures[idx].getSpec()[1], textures[idx].getSpec()[2]);
			textures[idx].bindDiff();
			textures[idx].bindSpec();
			program.setInt("material.diffuse", 0);
			program.setInt("material.specular", 1);
			// glDrawElements(GL_TRIANGLES, it->second, GL_UNSIGNED_INT, 0);
		
			// std::cout << "drawing: " << it->second / 3 << "times." << std::endl;
			for (size_t i = 0; i * 3 < *it; i++) {
				program.set3Float("material.ambient", colorsA[i + colorIDX][0], colorsA[i + colorIDX][1], colorsA[i + colorIDX][2]);
				program.set3Float("material.diff", colorsD[i + colorIDX][0], colorsD[i + colorIDX][1], colorsD[i + colorIDX][2]);
				// std::cout << "Color[0]: " << colors[i + colorIDX][0] << " Color[1]: " << colors[i + colorIDX][1] << " Color[2]: " << colors[i + colorIDX][2] << std::endl;
				glDrawArrays(GL_TRIANGLES, i * 3 + prevSize, 3);
			}
			colorIDX += *it / 3;
		}
		else {
			textures[idx].bindDiff();
			textures[idx].bindSpec();
			program.setInt("material.diffuse", 0);
			program.setInt("material.specular", 1);
			glDrawArrays(GL_TRIANGLES, prevSize, *it);

			// std::cout << texture.getSpecular().getID() << "/" << texture.getDiffuse().getID() << std::endl;
		}
		idx++;
		prevSize = *it;
	}
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

void Mesh::normalizeV(float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
	float scaleX = 1.6f / (maxX - minX);
	float scaleY = 1.6f / (maxY - minY);
	float scaleZ = 1.6f / (maxZ - minZ);
	float finalScale = scaleX;

	if (finalScale > scaleY)
		finalScale = scaleY;
	if (finalScale > scaleZ)
		finalScale = scaleZ;


	float centerX = 0.0f;
	float centerY = 0.0f;
	float centerZ = 0.0f;
		
	centerX = (maxX + minX) / 2;
	centerY = (maxY + minY) / 2;
	centerZ = (maxZ + minZ) / 2;
	center(-centerX, -centerY, -centerZ);

	if (!textured)
		normalizeT(maxX, maxY, maxZ, minX, minY, minZ);
	scaleMat = scaleMat.scale(Vector3(finalScale, finalScale, finalScale));
	Matrix4 model = scaleMat.mulMatrix(transMat);
	for (size_t i = 0; i < vertices.size(); i++) {
		Vector4 v(vertices[i].Position);
		v = v.mulMatrix(model);
		vertices[i].Position = Vector3(v[0], v[1], v[2]);
	}

	setupMesh();

}

void Mesh::center(float centerX, float centerY, float centerZ)
{
	transMat = transMat.translation(Vector3(centerX, centerY, centerZ));
}

float max(float a, float b, float c, float d, float e, float f) {
	float max = a;

	if (b > max)
		max = b;
	if (c > max)
		max = c;
	if (d > max)
		max = d;
	if (e > max)
		max = e;
	if (f > max)
		max = f;
	return max;
}

void Mesh::normalizeT(float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
	Vector3 up(0.0f, 1.0f, 0.0f);
	Vector3 down(0.0f, -1.0f, 0.0f);
	Vector3 left(-1.0f, 0.0f, 0.0f);
	Vector3 right(1.0f, 0.0f, 0.0f);
	Vector3 forward(0.0f, 0.0f, 1.0f);
	Vector3 back(0.0f, 0.0f, -1.0f);

	for (size_t i = 0; i < vertices.size(); i++) {

		float dotUp = vertices[i].Normal.dotVector(up);
		float dotDown = vertices[i].Normal.dotVector(down);
		float dotLeft = vertices[i].Normal.dotVector(left);
		float dotRight = vertices[i].Normal.dotVector(right);
		float dotForward = vertices[i].Normal.dotVector(forward);
		float dotBack = vertices[i].Normal.dotVector(back);

		float maxDot = max(dotUp, dotDown, dotLeft, dotRight, dotForward, dotBack);

		float normX = (vertices[i].Position[0] - minX) / (maxX - minX);
		float normY = (vertices[i].Position[1] - minY) / (maxY - minY);
		float normZ = (vertices[i].Position[2] - minZ) / (maxZ - minZ);

		if (maxDot == dotUp) {
			vertices[i].TexCoords[0] = normX;
			vertices[i].TexCoords[1] = normZ;
		} else if (maxDot == dotDown) {
			vertices[i].TexCoords[0] = normX;
			vertices[i].TexCoords[1] = normZ;
		} else if (maxDot == dotLeft) {
			vertices[i].TexCoords[0] = normY;
			vertices[i].TexCoords[1] = normZ;
		} else if (maxDot == dotRight) {
			vertices[i].TexCoords[0] = normY;
			vertices[i].TexCoords[1] = normZ;
		} else if (maxDot == dotForward) {
			vertices[i].TexCoords[0] = normX;
			vertices[i].TexCoords[1] = normY;
		} else if (maxDot == dotBack) {
			vertices[i].TexCoords[0] = normX;
			vertices[i].TexCoords[1] = normY;
		}

		// std::cout << "Textures Coords: " << vertices[i].TexCoords[0] << ", " << vertices[i].TexCoords[1] << std::endl;
	}

}

void Mesh::setIsTextured(bool isTextured)
{
	for (unsigned int i = 0; i < textures.size(); i++)
		textures[i].setIsTextured(isTextured);
}

void Mesh::setTex(bool Tex)
{
	textured = Tex;
}

void Mesh::setRotation(int axis)
{
	this->axis = axis;
	if (axis == 0)
		rotateMat = rotateMat.rotate(glfwGetTime() * 90.0f, Vector3(1.0f, 0.0f, 0.0f));
	if (axis == 1)
		rotateMat = rotateMat.rotate(glfwGetTime() * 90.0f, Vector3(0.0f, 1.0f, 0.0f));
	if (axis == 2)
		rotateMat = rotateMat.rotate(glfwGetTime() * 90.0f, Vector3(0.0f, 0.0f, 1.0f));
	if (axis == 3)
		rotateMat = Matrix4(1.0f);
}

void Mesh::setMove(int axis, float move)
{
	if (axis == 0)
		moveX += move;
	if (axis == 1)
		moveY += move;
	if (axis == 2)
		moveZ += move;
	moveMat = moveMat.translation(Vector3(moveX, moveY, moveZ));
}

bool Mesh::getTex(void) const
{
	return textured;
}

void Mesh::clear()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	for (size_t i = 0; i < textures.size(); i++)
		textures[i].clear();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

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
	glBindVertexArray(0);

	int idx = 0;
	srand (static_cast <unsigned> (time(0)));
	for (std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); it++) {

		if (textures[idx].getIsTextured() == false) {
		
			for (size_t i = 0; i * 3 < *it; i++) {
				float random =  -1.0f + static_cast <float> (rand()) / static_cast <float> ((float)RAND_MAX / 2.0f);

				colorsA.push_back(Vector3(textures[idx].getAmbient()[0] + random, textures[idx].getAmbient()[1] - random, textures[idx].getAmbient()[2]));
				colorsD.push_back(Vector3(textures[idx].getDiff()[0] + random, textures[idx].getDiff()[1] - random, textures[idx].getDiff()[2]));
				// std::cout << "Color[0]: " << textures[idx].getAmbient()[0] << " Color[1]: " << textures[idx].getAmbient()[1] << " Color[2]: " << textures[idx].getAmbient()[2] << std::endl;
			}
		}
		idx++;
	}
}



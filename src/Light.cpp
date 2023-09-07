#include "Light.hpp"

float vertices[] = {
    // positions
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

Light::Light(Vector3 position) : lightProgram(Shader("./shaders/lightShader.vert", "./shaders/lightShader.frag")), position(position)
{
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	unsigned int lightVBO;
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	
	glEnableVertexAttribArray(0);

	color = Vector3(1.0f, 1.0f, 1.0f);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.2f, 0.2f, 0.2f);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Light::use() const
{
	lightProgram.use();
}

void Light::draw(Matrix4 view, Matrix4 projection) const
{
	Matrix4 lightModel = lightModel.model(lightModel.translation(position), lightModel.scale(Vector3(0.2f, 0.2f, 0.2f)));;
	float transMatrix[16];

	lightProgram.use();

	lightModel.getMatrix(transMatrix);
	lightProgram.setMat4("model", transMatrix);
		
	projection.getMatrix(transMatrix);
	lightProgram.setMat4("projection", transMatrix);

	view.getMatrix(transMatrix);
	lightProgram.setMat4("view", transMatrix);
		
	lightProgram.set3Float("color", color[0], color[1], color[2]);
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

Vector3 Light::getLightPos() const
{
	return position;
}

Vector3 Light::getLightColor() const
{
	return color;
}

Vector3 Light::getLightAmb() const
{
	return ambient;
}

Vector3 Light::getLightDiff() const
{
	return diffuse;
}

void Light::setLightPos(Vector3 pos)
{
	position = pos;
}

void Light::setLightColor(Vector3 col)
{
	color = col;
}

void Light::setLightAmb(Vector3 amb)
{
	ambient = amb;
}

void Light::setLightDiff(Vector3 diff)
{
	diffuse = diff;
}

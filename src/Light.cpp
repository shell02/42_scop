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

Light::Light(Vector3 position) : lightProgram(Shader("./shaders/lightShader.vert", "./shaders/lightShader.frag")), position(position), err(0)
{
    if (lightProgram.getErr()) {
        err = 1;
        return ;
    }
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);	
	glEnableVertexAttribArray(0);

	color = Vector3(1.0f, 1.0f, 1.0f);
	ambient = Vector3(0.2f, 0.2f, 0.2f);
	diffuse = Vector3(0.4f, 0.4f, 0.4f);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Light::~Light() {}

void Light::clear() {
    glDeleteProgram(lightProgram.getId());
    glDeleteBuffers(1, &lightVBO);
    glDeleteVertexArrays(1, &lightVAO);
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
    if (color[0] == 1.0f && color[1] == 1.0f && color[2] == 1.0f) {
        // setLightAmb(Vector3(0.2f, 0.2f, 0.2f));
        setLightDiff(Vector3(0.4f, 0.4f, 0.4f));
    } else {
        setLightDiff(color.mulScalar(0.2f));
        // setLightAmb(color.mulScalar(0.4f));
    }
}

void Light::setLightAmb(Vector3 amb)
{
	ambient = amb;
}

void Light::setLightDiff(Vector3 diff)
{
	diffuse = diff;
}

int Light::getErr() const
{
    return err;
}

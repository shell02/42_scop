#include "scop.h"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include "Model.hpp"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <vector>

float lastX = 400;
float lastY = 300;
float yaw = -90.0f;
float pitch = 0;
bool firstMouse = true;

int main()
{

	GLFWwindow* window;


	if (!glfwInit()) {
		std::cerr << "ERROR\nCould not start GLFW!" << std::endl;
		return -1;
	}
	
	// INIT LOG
	assert(restart_gl_log());
	gl_log("Starting GLFW\n%s\n", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Tuto", NULL, NULL);
	if (window == NULL) {
		gl_log_err("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gl_log("Window of size %dpx by %dpx created and in context.\n", WIDTH, HEIGHT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		gl_log_err("Failed to initialize GLAD\n");
		return -1;
	}
	gl_log("GLAD initialized\n");

	// LOG GL_ERRORS
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, NULL);
	
	// WINDOW RESIZING
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

// float vertices[] = {
//     // positions          // normals           // texture coords
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
// };

	// Vector3 cubePositions[] = {
    //     Vector3( 0.0f,  0.0f,  0.0f),
    //     Vector3( 2.0f,  5.0f, -15.0f),
    //     Vector3(-1.5f, -2.2f, -2.5f),
    //     Vector3(-3.8f, -2.0f, -12.3f),
    //     Vector3( 2.4f, -0.4f, -3.5f),
    //     Vector3(-1.7f,  3.0f, -7.5f),
    //     Vector3( 1.3f, -2.0f, -2.5f),
    //     Vector3( 1.5f,  2.0f, -2.5f),
    //     Vector3( 1.5f,  0.2f, -1.5f),
    //     Vector3(-1.3f,  1.0f, -1.5f)
    // };

	// Texture diffuseMap("./assets/container2.png");
	// Texture specularMap("./assets/container2_specular.png");


	Model model("./assets/42.obj");

	if (model.getErr()) {
		gl_log_err("**ERROR** : Could not initialize model\n");
		exit(1);
	}

	// std::vector<Mesh> obj = model.getObjects();
	// std::vector<Vertex> vert = obj[0].vertices;

	// unsigned int vbo;
	// glGenBuffers(1, &vbo);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(Vertex), &vert[0], GL_STATIC_DRAW);

	// unsigned int vao;
	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);

	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));	
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));	
	// glEnableVertexAttribArray(2);


	Light light;

	Shader program("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");

	Camera camera;


	float transMatrix[16];

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
	glfwSetCursorPosCallback(window, mouse_callback); 

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.setDeltaTime(deltaTime);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		light.setLightPos(Vector3(sin(currentFrame) * 3.0f, 1.0f, cos(currentFrame) * 3.0f));
		light.setLightColor(Vector3(1.0f, 1.0f, 1.0f));

		program.use();

		Matrix4 view = camera.LookAt();
		// Matrix4 models(1.0f);
		Matrix4 projection(1.0f);

		// models.getMatrix(transMatrix);
		// program.setMat4("model", transMatrix);

		projection = projection.perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
		projection.getMatrix(transMatrix);
		program.setMat4("projection", transMatrix);

		view.getMatrix(transMatrix);
		program.setMat4("view", transMatrix);
		program.set3Float("light.position", light.getLightPos()[0], light.getLightPos()[1], light.getLightPos()[2]);
		program.set3Float("viewPos", camera.getPos()[0], camera.getPos()[1], camera.getPos()[2]);
		

		Vector3 ambientColor = light.getLightAmb();
		Vector3 diffuseColor = light.getLightDiff();
		
		// glActiveTexture(GL_TEXTURE0);
		// obj[0].texture.bindDiff();
		// glActiveTexture(GL_TEXTURE1);
		// obj[0].texture.bindSpec();
		
		// program.setInt("material.diffuse", 0);
		// program.setInt("material.specular", 1);
		// program.setFloat("material.shininess", 32.0f);
		// program.setFloat("material.transparency", 1.0f);
		// program.setBool("isTextured", true);
		// program.set3Float("light.direction", -0.2f, -0.1f, -0.3f);
		program.set3Float("light.ambient", ambientColor[0], ambientColor[1], ambientColor[2]);
		program.set3Float("light.diffuse", diffuseColor[0], diffuseColor[1], diffuseColor[2]);
		program.set3Float("light.specular", 1.0f, 1.0f, 1.0f);

		model.draw(program);

		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, diffuseMap.getID());

		// glBindVertexArray(vao);
        // for (unsigned int i = 0; i < 10; i++)
        // {
        //     // calculate the model matrix for each object and pass it to shader before drawing
        //     Matrix4 model = Matrix4(1.0f);
        //     model = model.translation(cubePositions[i]);
        //     float angle = 20.0f * i;
        //     model = model.mulMatrix(model.rotate(angle, Vector3(1.0f, 0.3f, 0.5f)));
		// 	model.getMatrix(transMatrix);
        //     program.setMat4("model", transMatrix);

        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

		// obj[0].draw(program);
		// glDrawArrays(GL_TRIANGLES, 0, 36);

		light.draw(view, projection);		

		glfwSwapBuffers(window);
    	glfwPollEvents();

		//WASD Movements
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.moveFore();
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.moveBack();
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.moveLeft();
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.moveRight();

		Vector3 direction(
			cos(toRadians(yaw)) * cos(toRadians(pitch)),
			sin(toRadians(pitch)),
			sin(toRadians(yaw)) * cos(toRadians(pitch))
		);
		camera.updateFront(direction.unit());

	}

	glfwTerminate();

	return 0;
}


		// Matrix4 lightTrans = lightView.transpose();
		// std::cout << "LightModel:\n";
		// for (int i = 0; i < 4; i++) {
		// 	for (int j = 0; j < 4; j++) {
		// 		std::cout << lightTrans[i*4+j] << ", ";
		// 	}
		// 	std::cout << std::endl;
		// }

		// trans = gView;
		// std::cout << "GLM:\n";
		// for (int i = 0; i < 4; i++) {
		// 	for (int j = 0; j < 4; j++) {
		// 		std::cout << trans[i][j] << ", ";
		// 	}
		// 	std::cout << std::endl;
		// }
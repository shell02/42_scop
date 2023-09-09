#include "scop.h"
#include "Vector.hpp"
#include "Matrix.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include "Model.hpp"

#include <vector>

float lastX = 400;
float lastY = 300;
float yaw = -90.0f;
float pitch = 0;
bool firstMouse = true;

int checkRessources(void) {
	struct stat sb;

	if (stat("./assets/kittens.jpg", &sb) != 0) {
		std::cerr << "Missing file './assets/kittens.jpg\n" << std::endl;
		return 0;
	}
	if (stat("./assets/white_spec.png", &sb) != 0) {
		std::cerr << "Missing file './assets/white_spec.png\n" << std::endl;
		return 0;
	}
	if (stat("./shaders/fragmentShader.frag", &sb) != 0) {
		std::cerr << "Missing file './shaders/fragmentShader.frag\n" << std::endl;
		return 0;
	}
	if (stat("./shaders/vertexShader.vert", &sb) != 0) {
		std::cerr << "Missing file './shaders/vertexShader.vert\n" << std::endl;
		return 0;
	}
	if (stat("./shaders/lightShader.frag", &sb) != 0) {
		std::cerr << "Missing file './shaders/lightShader.frag\n" << std::endl;
		return 0;
	}
	if (stat("./shaders/lightShader.vert", &sb) != 0) {
		std::cerr << "Missing file './shaders/lightShader.vert\n" << std::endl;
		return 0;
	}
	
	return 1;
}

int main(int argc, char **argv)
{

	std::string filename;

	if (argc != 2) {
		std::cerr << "**ERROR** : Please provide an .obj file.\nUsage: ./scop [file.obj]\n";
		exit(1);
	}
	else
		filename = std::string(argv[1]);

	if (!checkRessources()) {
		exit(1);
	}

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

	window = glfwCreateWindow(WIDTH, HEIGHT, "42 ft_scop", NULL, NULL);
	if (window == NULL) {
		gl_log_err("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gl_log("Window of size %dpx by %dpx created and in context.\n", WIDTH, HEIGHT);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		gl_log_err("Failed to initialize GLAD\n");
		glfwTerminate();
		return -1;
	}

	// LOG GL_ERRORS
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_error_callback, NULL);
	
	// WINDOW RESIZING
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Model model(filename);
	if (model.getErr()) {
		gl_log_err("**ERROR** : Could not initialize model\n");
		glfwTerminate();
		exit(1);
	}

	Light light;

	Camera camera;

	Shader program("./shaders/vertexShader.vert", "./shaders/fragmentShader.frag");

	//Handle Mouse Moving 

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	float visual = 1.0f;
	float inc = 0.01f;
	float transMatrix[16];

	Vector3 lightPos(3.0f, 3.0f, 3.0f);
	Vector3 lightColor(1.0f, 1.0f, 1.0f);
	
	Matrix4 projection(1.0f);
	projection = projection.perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	controls ctrl;

	std::cout << "CONTROLS :\n"
			  << "Object Rotation	:	X	Y	Z	H (no rotation)\n"
			  << "Object Movement	:	ARROWS	B(ackward)	F(orward)\n\n"
			  << "Camera Movement	:	M (de/activate)	WASD	Mouse\n\n"
			  << "Texture on/off		:	T\n"
			  << "Wires on/off		:	L\n"
			  << "Colored light on/off	:	P\n" 
			  <<std::endl;

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.setDeltaTime(deltaTime);

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (ctrl.party) {
			lightPos = Vector3(sin(currentFrame) * 1.5f, 0.0f, cos(currentFrame) * 1.5f);
			lightColor = Vector3(sin(currentFrame) / 2 + 0.5f, 0.5f, 0.6f);
		} else {
			lightPos = Vector3(2.0f, 2.0f, 2.0f);
			lightColor = Vector3(1.0f, 1.0f, 1.0f);
		}
		light.setLightPos(lightPos);
		light.setLightColor(lightColor);

		program.use();

		Matrix4 view = camera.LookAt();

		projection.getMatrix(transMatrix);
		program.setMat4("projection", transMatrix);

		view.getMatrix(transMatrix);
		program.setMat4("view", transMatrix);
		program.set3Float("viewPos", camera.getPos()[0], camera.getPos()[1], camera.getPos()[2]);


		program.set3Float("light.position", light.getLightPos()[0], light.getLightPos()[1], light.getLightPos()[2]);
		program.set3Float("light.ambient", light.getLightAmb()[0], light.getLightAmb()[1], light.getLightAmb()[2]);
		program.set3Float("light.diffuse", light.getLightDiff()[0], light.getLightDiff()[1], light.getLightDiff()[2]);
		program.set3Float("light.specular", 1.0f, 1.0f, 1.0f);


		model.draw(program);

		light.draw(view, projection);

		program.use();
		if (visual > 0.0f && visual < 1.0f) {
			visual += inc;
			program.setFloat("transition", visual);
			model.setTexture(false);
		}
		if (visual >= 1.0f)
			model.setTexture(false);
		if (visual <= 0.0f)
			model.setTexture(true);

		glfwSwapBuffers(window);
    	glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
       		glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			if (ctrl.wires)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			ctrl.wires = ctrl.wires ? false : true;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			if (visual <= 0.0f || visual >= 1.0f) {

				if (inc >= 0.01f)
					inc = -0.01f;
				else
					inc = 0.01f;
				visual += inc;
			}

		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
			ctrl.party = ctrl.party ? false : true;
		}
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
			if (ctrl.move) {
				glfwSetCursorPosCallback(window, NULL);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			} else {
				glfwSetCursorPosCallback(window, mouse_callback);
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			ctrl.move = ctrl.move ? false : true;
		}

		if (ctrl.move) 
		{
			//WASD Movements
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				camera.moveFore();
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				camera.moveBack();
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				camera.moveLeft();
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				camera.moveRight();
			
			//Mouse Movements
			Vector3 direction(
				cos(toRadians(yaw)) * cos(toRadians(pitch)),
				sin(toRadians(pitch)),
				sin(toRadians(yaw)) * cos(toRadians(pitch))
			);
			camera.updateFront(direction.unit());
		}

		if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
			model.setRotation(0);
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			model.setRotation(1);
		}
		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
			model.setRotation(2);
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
			model.setRotation(3);
		}

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			model.setMove(1, 0.05f);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			model.setMove(1, -0.05f);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			model.setMove(0, -0.05f);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			model.setMove(0, 0.05f);;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			model.setMove(2, 0.05f);
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
			model.setMove(2, -0.05f);
	}

	// glDeleteProgram(program.getId());
	glfwTerminate();

	return 0;
}
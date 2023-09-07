#ifndef _SCOP_H_
#define _SCOP_H_

// OpenGL Librairies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#define GL_LOG_FILE "gl.log"
#define WIDTH 800
#define HEIGHT 600

#define PI 3.14159265359


// LOG FUNCTIONS
bool restart_gl_log();
bool gl_log(const char *message, ...);
bool gl_log_err(const char *message, ...);
void glfw_error_callback(int error, const char *description);
void gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
unsigned int severity, int length, const char* message, const void* userParam);

// UTILITY FUNCTIONS
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float toRadians(float f);

#endif
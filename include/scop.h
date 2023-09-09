#ifndef _SCOP_H_
#define _SCOP_H_

// OpenGL Librairies
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <vector>

#include <sys/stat.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define GL_LOG_FILE "gl.log"
#define WIDTH 800
#define HEIGHT 600

#define PI 3.14159265359

typedef struct controls {
	bool wires = false;
	bool textured = false;
	bool party = false;
	bool move = false;
} controls;

// LOG FUNCTIONS
bool restart_gl_log();
bool gl_log(const char *message, ...);
bool gl_log_err(const char *message, ...);
void glfw_error_callback(int error, const char *description);
void gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
unsigned int severity, int length, const char* message, const void* userParam);

// UTILITY FUNCTIONS
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
float toRadians(float f);

#endif
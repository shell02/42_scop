#include "scop.h"

bool restart_gl_log() {
	std::fstream file;
	file.open(GL_LOG_FILE, std::ios::out | std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for writing\n"; 
		return false;
	}
	time_t now = time(NULL);
	char *date = ctime(&now);
	file << GL_LOG_FILE << " log. local time " << date << std::endl;
	file.close();
	return true;
}

bool gl_log(const char *message, ...)
{
	va_list argptr;
	std::fstream file;
	char buffer[500];
	file.open(GL_LOG_FILE, std::ios::out | std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for appending\n"; 
		return false;
	}
	va_start(argptr, message);
	vsprintf(buffer, message, argptr);
	va_end(argptr);
	file << buffer;
	file.close();
	return true;
}

bool gl_log_err(const char *message, ...)
{
	va_list argptr;
	std::fstream file;
	char buffer[500];
	file.open(GL_LOG_FILE, std::ios::out | std::ios::app);
	if (!file.is_open())
	{
		std::cerr << "ERROR: could not open GL_LOG_FILE log file " << GL_LOG_FILE << " for appending\n"; 
		return false;
	}
	va_start(argptr, message);
	vsprintf(buffer, message, argptr);
	va_end(argptr);
	file << buffer;
	file.close();
	std::cerr << buffer;
	return true;
}

void glfw_error_callback(int error, const char *description) {
	gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void gl_error_callback(unsigned int source, unsigned int type, unsigned int id,
   unsigned int severity, int length, const char* message, const void* userParam) {
	(void)source;
	(void)id;
	(void)severity;
	(void)length;
	(void)userParam;
	gl_log_err("GL CALLBACK: %s, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "** OTHER **" ), message );
}
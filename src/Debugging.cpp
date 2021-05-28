#include "Debugging.hpp"
#include "glew/include/GL/glew.h"
#include <iostream>

// clear any remaining error
void GLClearError() {

	while (glGetError() != GL_NO_ERROR) {
	}
}

// print the error if happened
bool GLLogCall(const char *function, const char *file, int line) {
	GLenum e;
	while (e = glGetError()) {
		std::cout << "[GL error] (" << e << "): at function " << function << ": at file " << file << ": at line " << line << std::endl;
		return false;
	}
	return true;
}
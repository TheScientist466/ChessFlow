#include <glad/glad.h>
#include <plog/Log.h>

#include "ChessFlow/HandleError.h"

void GLClearError() {
	while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while(GLenum error = glGetError()) {
		PLOGE << "[OpenGL Error] (" << error << ")";
		return false;
	}
	return true;
}
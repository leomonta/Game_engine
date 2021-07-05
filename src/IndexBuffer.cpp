#include "IndexBuffer.hpp"
#include "Debugging.hpp"
#include "glew/include/GL/glew.h"

// create the index buffer and fill
unsigned int IndexBuffer::createBuffer(int count) {
	unsigned int Buffer_ID;
	GLCall(glGenBuffers(1, &Buffer_ID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_ID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW));

	return Buffer_ID;
}

void IndexBuffer::deleteBuffer(unsigned int Buffer_ID) {
	GLCall(glDeleteBuffers(1, &Buffer_ID));
}

void IndexBuffer::submitData(unsigned int Buffer_ID, unsigned int count, const void *data) {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer_ID));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
}
#include "VertexBuffer.hpp"
#include "Renderer.hpp"

// create a vertex buffer on the GPU
unsigned int VertexBuffer::createBuffer(unsigned int size) {

	unsigned int Buffer_ID;
	// Gen empty buffer (how many, where)
	GLCall(glGenBuffers(1, &Buffer_ID));
	// specify the purpose of the buffer by binding it
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, Buffer_ID));
	// fill the buffer with the given data, and set the buffer to be written once and used many times
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));

	return Buffer_ID;
}

// delete the Vertex buffer from the GPU
void VertexBuffer::deleteBuffer(unsigned int Buffer_ID) {
	GLCall(glDeleteBuffers(1, &Buffer_ID));
}

void VertexBuffer::submitData(unsigned int Buffer_ID, unsigned int size, const void *data) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, Buffer_ID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
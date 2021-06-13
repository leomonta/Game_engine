#include "VertexBuffer.hpp"
#include "Renderer.hpp"

// create a vertex buffer on the GPU
VertexBuffer::VertexBuffer(int size) {
	// Gen empty buffer (how many, where)
	GLCall(glGenBuffers(1, &m_RendererID));
	// specify the purpose of the buffer by binding it
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	// fill the buffer with the given data, and set the buffer to be written once and used many times
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	m_size = size;
}

// delete the Vertex buffer from the GPU
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::submitData(const void *data) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_size, data));
}
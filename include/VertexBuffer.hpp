#pragma once

#include "Debugging.hpp"

class VertexBuffer {
public:
	// the unique ID of the buffer
	unsigned int m_RendererID;
	unsigned int m_size;

	VertexBuffer(int size);
	~VertexBuffer();

	void submitData(const void *data);
};

#pragma once
#include <cstring>

#include "VertexBuffer.hpp"
#include "Debugging.hpp"
#include "glm/glm.hpp"

class VertexArray {
public:
	unsigned int m_RendererID;

	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb);
};

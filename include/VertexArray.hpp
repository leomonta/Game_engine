#pragma once
#include <cstring>

#include "VertexBuffer.hpp"
#include "glm/glm.hpp"

struct Vertex {
	static const int size = 3 + 2 + 3+ 4 + 1;

	glm::vec3 Vert_position;
	glm::vec2 Tex_coord;
	glm::vec3 Normal;
	glm::vec4 Tint;
	float	  Tex_ID;

	void data(float *vertex_buffer);
};

class VertexArray {
public:
	unsigned int m_RendererID;

	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb);
};

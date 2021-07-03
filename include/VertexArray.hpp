#pragma once
#include <cstring>

#include "Debugging.hpp"
#include "VertexBuffer.hpp"
#include "glm/glm.hpp"

class VertexArray {
public:
	static unsigned int createBuffer();
	static void			deleteBuffer(const unsigned int Buffer_ID);

	static void AddBuffer(const unsigned int VArray, const unsigned int VBuffer);
};

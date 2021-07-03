#pragma once

#include "Debugging.hpp"

class VertexBuffer {
public:
	static unsigned int createBuffer(unsigned int size);
	static void			deleteBuffer(unsigned int Buffer_ID);

	static void submitData(unsigned int Buffer_ID, unsigned int size, const void *data);
};

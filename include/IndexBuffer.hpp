#pragma once

class IndexBuffer {
public:
	static unsigned int createBuffer(const unsigned int* data, int count);
	static void deleteBuffer(unsigned int Buffer_ID);
};

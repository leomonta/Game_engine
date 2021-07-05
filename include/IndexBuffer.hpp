#pragma once

class IndexBuffer {
public:
	static unsigned int createBuffer(int count);
	static void deleteBuffer(unsigned int Buffer_ID);


	static void submitData(unsigned int Buffer_ID, unsigned int size, const void *data);
};

#include "VertexArray.hpp"
#include "Renderer.hpp"

// create a Vertex array on the GPU, but no data
unsigned int VertexArray::createBuffer() {
	unsigned int Buffer_ID;
	GLCall(glGenVertexArrays(1, &Buffer_ID));

	return Buffer_ID;
}

// delete the Vertex Array from the GPU
void VertexArray::deleteBuffer(const unsigned int Buffer_ID) {
	GLCall(glDeleteVertexArrays(1, &Buffer_ID));
}

// add a buffer and appy the given layout
void VertexArray::AddBuffer(const unsigned int VArray, const unsigned int VBuffer) {
	//	| attribute0(2)| attribute1(2)				      |
	//	| pos.x, pos.y | texture coord.x, texture coord.y |
	//	|			   | component 1 ,		 component 2  |
	//  ------------------The Entire Vertex----------------

	Renderer::BindVertexArray(VArray);
	Renderer::BindVertexBuffer(VBuffer);

	// offset from the start of the vertex
	int Vert_size = sizeof(Vertex);

	// say to openGL the index, in the shader layout=attributeIndex
	GLCall(glEnableVertexAttribArray(0));

	// 0 which attribute
	// 1 how many components the attribute have
	// 2 data type of the components, the components of an attribute MUST be all of the same type obviously
	// 3 if it needs to be normalized
	// 4 size of the entire vertex in bytes
	// 5 offset from the start of the vertex to the start of the attribute 0 -> 12 -> 18 -> ...

	//							|0| 1| 2	   | 3| 4			  | 5
	// Vertex position
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, 0, Vert_size, (const void *)offsetof(Vertex, Vert_position)));

	// Texture coordinte, vec2
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, 0, Vert_size, (const void *)offsetof(Vertex, Tex_coord)));

	// Texture coordinate, Normal
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, 0, Vert_size, (const void *)offsetof(Vertex, Normal)));

	// Tint or color
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, 0, Vert_size, (const void *)offsetof(Vertex, Tint)));

	// Texture slot id
	GLCall(glEnableVertexAttribArray(4));
	GLCall(glVertexAttribPointer(4, 1, GL_FLOAT, 0, Vert_size, (const void *)offsetof(Vertex, Tex_ID)));
}
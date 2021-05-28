#include "VertexArray.hpp"
#include "Renderer.hpp"

// create a Vertex array on the GPU, but no data
VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

// delete the Vertex Array from the GPU
VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

// add a buffer and appy the given layout
void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexLayout &layout) {
	//	| attribute0(2)| attribute1(2)				      |
	//	| pos.x, pos.y | texture coord.x, texture coord.y |
	//	|			   | component 1 ,		 component 2  |
	//  ------------------The Entire Vertex----------------

	Renderer::BindVertexArray(*this);
	Renderer::BindVertexBuffer(vb);

	// all the attributes of a vertex
	const auto &attributes = layout.GetElements();

	// offset from the start of the vertex
	unsigned int* offset = 0;

	for (unsigned int attributeIndex = 0; attributeIndex < attributes.size(); attributeIndex++) {

		// get all the componentes
		const auto &element = attributes[attributeIndex];

		// say to openGL the index, in the shader layout=attributeIndex
		GLCall(glEnableVertexAttribArray(attributeIndex));

		// 0 which attribute
		// 1 how many components the attribute have
		// 2 data type of the components, the components of an attribute MUST be all of the same type obviously
		// 3 if it needs to be normalized
		// 4 size of the entire vertex in bytes
		// 5 offset from the start of the vertex to the start of the attribute 0 -> 12 -> 18 -> ...

		//							| 0			   | 1			  | 2			| 3					| 4					| 5
		GLCall(glVertexAttribPointer(attributeIndex, element.count, element.type, element.normalized, layout.GetStride(), (const void *)(offset)));

		// update the offset
		*offset += element.count * VertexAttribute::GetSizeOfType(element.type);
	}
}
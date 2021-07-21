#include "renderer.hpp"
#include "Debugging.hpp"
#include "profiler.hpp"

void Vertex::data(float *vertex_buffer) {

	assert(vertex_buffer[size - 1]);

	memcpy(vertex_buffer, this, size * sizeof(float));
}

Renderer::Renderer() {

	Current_batch.VertBuffer = new Vertex[MAX_VERTEX_COUNT];
	Current_batch.IndxBuffer = new unsigned int[MAX_VERTEX_COUNT];
	Current_batch.nextVert	 = Current_batch.VertBuffer;

	// create the necessary buffers
	Current_batch.GLVertexArray	 = VertexArray::createBuffer();
	Current_batch.GLVertexBuffer = VertexBuffer::createBuffer(sizeof(Vertex) * MAX_VERTEX_COUNT);
	Current_batch.GLIndexBuffer	 = IndexBuffer::createBuffer(MAX_VERTEX_COUNT);

	Current_batch.indexCount = 0;
	Current_batch.indexVal	 = 0;

	// apply the Vertex attributes to correct Array and buffer
	VertexArray::AddBuffer(Current_batch.GLVertexArray, Current_batch.GLVertexBuffer);
}

Renderer::~Renderer() {
	delete[] Current_batch.VertBuffer;
	delete[] Current_batch.IndxBuffer;

	glDeleteBuffers(1, &Current_batch.GLIndexBuffer);
	glDeleteBuffers(1, &Current_batch.GLVertexBuffer);
	glDeleteVertexArrays(1, &Current_batch.GLVertexArray);
}

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::DrawVertex(Vertex &V) {
	(*Current_batch.nextVert) = V;

	Current_batch.IndxBuffer[Current_batch.indexCount] = Current_batch.indexVal;

	Current_batch.indexVal++;
	Current_batch.nextVert++;
	Current_batch.indexCount++;
}

void Renderer::DrawTris(Vertex &v1, Vertex &v2, Vertex &v3) {

	if (Current_batch.indexCount + 4 > MAX_VERTEX_COUNT) {
		Commit();
	}

	(*Current_batch.nextVert) = v1;
	Current_batch.nextVert++;

	(*Current_batch.nextVert) = v2;
	Current_batch.nextVert++;

	(*Current_batch.nextVert) = v3;
	Current_batch.nextVert++;

	Current_batch.IndxBuffer[Current_batch.indexCount + 0] = Current_batch.indexVal + 0;
	Current_batch.IndxBuffer[Current_batch.indexCount + 1] = Current_batch.indexVal + 1;
	Current_batch.IndxBuffer[Current_batch.indexCount + 2] = Current_batch.indexVal + 2;
	Current_batch.indexCount += 3;
	Current_batch.indexVal += 3;
}

void Renderer::DrawQuad(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4) {

	if (Current_batch.indexCount + 4 > MAX_VERTEX_COUNT) {
		Commit();
	}
	(*Current_batch.nextVert) = v1;
	Current_batch.nextVert++;

	(*Current_batch.nextVert) = v2;
	Current_batch.nextVert++;

	(*Current_batch.nextVert) = v3;
	Current_batch.nextVert++;

	(*Current_batch.nextVert) = v4;
	Current_batch.nextVert++;

	Current_batch.IndxBuffer[Current_batch.indexCount + 0] = Current_batch.indexVal + 0;
	Current_batch.IndxBuffer[Current_batch.indexCount + 1] = Current_batch.indexVal + 1;
	Current_batch.IndxBuffer[Current_batch.indexCount + 2] = Current_batch.indexVal + 2;
	Current_batch.IndxBuffer[Current_batch.indexCount + 3] = Current_batch.indexVal + 2;
	Current_batch.IndxBuffer[Current_batch.indexCount + 4] = Current_batch.indexVal + 3;
	Current_batch.IndxBuffer[Current_batch.indexCount + 5] = Current_batch.indexVal + 0;
	Current_batch.indexVal += 4;
	Current_batch.indexCount += 6;
}

void Renderer::Commit() {

	BindIndexBuffer(Current_batch.GLIndexBuffer);
	BindVertexArray(Current_batch.GLVertexArray);
	BindVertexBuffer(Current_batch.GLVertexBuffer);

	// Get how many Vertex i need to send to the buffer
	unsigned int size = (unsigned int)(Current_batch.nextVert - Current_batch.VertBuffer) * (unsigned int)(sizeof(Vertex));

	// Upload vertex data to the GPU VertexBuffer
	VertexBuffer::submitData(Current_batch.GLVertexBuffer, size, Current_batch.VertBuffer);
	IndexBuffer::submitData(Current_batch.GLIndexBuffer, Current_batch.indexCount, Current_batch.IndxBuffer);

	GLCall(glDrawElements(GL_TRIANGLES, Current_batch.indexCount, GL_UNSIGNED_INT, nullptr));

	// reset the position I'm writing to
	Current_batch.nextVert	 = Current_batch.VertBuffer;
	Current_batch.indexCount = 0;
	Current_batch.indexVal	 = 0;
}

void Renderer::BindIndexBuffer(const unsigned int &IBuffer) {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuffer));
}

void Renderer::BindVertexBuffer(const unsigned int &VBuffer) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBuffer));
}

void Renderer::BindVertexArray(const unsigned int &VArray) {
	GLCall(glBindVertexArray(VArray));
}

void Renderer::BindTexture(const unsigned int &Texture2D, unsigned int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, Texture2D));
}

void Renderer::BindShaderProgram(const unsigned int &ShaderProgram) {
	GLCall(glUseProgram(ShaderProgram));
}

void Renderer::UnBindIndexBuffer() {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void Renderer::UnBindVertexBuffer() {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Renderer::UnBindVertexArray() {
	GLCall(glBindVertexArray(0));
}

void Renderer::UnBindTexture() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::UnBindShaderProgram() {
	GLCall(glUseProgram(0));
}

#include "renderer.hpp"
#include "Debugging.hpp"
#include "profiler.hpp"
#include <iostream>

void Renderer::Clear() const {
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) {
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::BindIndexBuffer(const IndexBuffer &IBuffer) {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBuffer.m_RendererID));
}

void Renderer::BindVertexBuffer(const VertexBuffer &VBuffer) {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBuffer.m_RendererID));
}

void Renderer::BindVertexArray(const VertexArray &VArray) {
	GLCall(glBindVertexArray(VArray.m_RendererID));
}

void Renderer::BindTexture(const Texture &Texture2D, unsigned int slot) {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, Texture2D.m_RendererID));
}

void Renderer::BindShaderProgram(const Shader &ShaderProgram) {
	GLCall(glUseProgram(ShaderProgram.m_RendererID));
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

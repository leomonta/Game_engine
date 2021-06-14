#pragma once

#include "glew/include/GL/glew.h"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

#define MAX_VERTEX_COUNT

class Renderer {
public:

	struct Batch {

	};

	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& sh);

	// select / bind specific buffers
	static void BindIndexBuffer(const IndexBuffer& IBuffer);
	static void BindVertexBuffer(const VertexBuffer& VBuffer);
	static void BindVertexArray(const VertexArray& VArray);
	static void BindTexture(const Texture& Texture2D, unsigned int slot);
	static void BindShaderProgram(const Shader& ShaderProgram);

	// deselect / unbind specific buffers 
	static void UnBindIndexBuffer();
	static void UnBindVertexBuffer();
	static void UnBindVertexArray();
	static void UnBindTexture();
	static void UnBindShaderProgram();
};
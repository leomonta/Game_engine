#pragma once

#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "glew/include/GL/glew.h"
#include "glm/glm.hpp"

#define MAX_VERTEX_COUNT 2048

struct Vertex {
	static const int size = 3 + 2 + 3 + 4 + 1;

	glm::vec3 Vert_position;
	glm::vec2 Tex_coord;
	glm::vec3 Normal;
	glm::vec4 Color;
	float	  Tex_ID;

	void data(float *vertex_buffer);
};

struct Batch { // represent the stuff that must be drawn
	Vertex *VertBuffer;
	Vertex *nextVert = VertBuffer;

	unsigned int VBuffer, VArray, IBuffer = 0;

	unsigned int* IndxBuffer;

	// how many index are used and the latest index number used
	int indexCount, indexVal;
};

class Renderer {
public:
	Batch Current_batch;

	Renderer();
	~Renderer();
	void Clear() const;
	void UseShader(const Shader &sh);

	// Draw Functions
	void DrawVertex(Vertex &V); // Add geometry to the batch
	void DrawTris(Vertex &v1, Vertex &v2, Vertex &v3);
	void DrawQuad(Vertex &v1, Vertex &v2, Vertex &v3, Vertex &v4);
	void Commit(); // draw the geometry in the batch

	// select / bind specific buffers
	static void BindIndexBuffer(const unsigned int &IBuffer);
	static void BindVertexBuffer(const unsigned int &VBuffer);
	static void BindVertexArray(const unsigned int &VArray);
	static void BindTexture(const unsigned int &Texture2D, unsigned int slot);
	static void BindShaderProgram(const unsigned int &ShaderProgram);

	// deselect / unbind specific buffers
	static void UnBindIndexBuffer();
	static void UnBindVertexBuffer();
	static void UnBindVertexArray();
	static void UnBindTexture();
	static void UnBindShaderProgram();
};
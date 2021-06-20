#pragma once

#include <string>
#include <unordered_map>

#include "Debugging.hpp"
#include "glm/glm.hpp"

class Shader {
public:
	// the unique ID of the buffer
	unsigned int						 m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	Shader();
	~Shader();

	// add a shader
	void addShader(const std::string &filenamefilepath, unsigned int type);

	// Set uniform
	void SetUniform1i(const std::string name, int value);
	void SetUniform1f(const std::string name, float value);
	void SetUniform3f(const std::string name, float v0, float v1, float v2);
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string name, const glm::mat4 &matrix);

private:
	unsigned int CompileShader(unsigned int type, const std::string &source);
	int			 GetUniformLocation(const std::string &name);
};
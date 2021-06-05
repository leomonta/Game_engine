#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Renderer.hpp"
#include "Shader.hpp"

// generate a shader program, a program will include multiple shaders
Shader::Shader() {
	m_RendererID = glCreateProgram();
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

// compile a shader and print compilation error
unsigned int Shader::CompileShader(unsigned int type, const std::string &source) {
	// create the vertex shader
	unsigned int id	 = glCreateShader(type);
	const char * src = source.c_str();

	// upload the shader code to the GPU
	GLCall(glShaderSource(id, 1, &src, nullptr));
	// actually compile the shader
	GLCall(glCompileShader(id));

	// get the compilation data
	int res;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &res));

	// if the compile status is false the compilation failed
	if (res == GL_FALSE) {

		// get the log length
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// get the log and allocate it on the stack
		char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		// print the compilation error
		std::cout << "[Shader]: Error: failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader"
				  << "\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return -1;
	}

	return id;
}

// add a shader that will be compiled
void Shader::addShader(const std::string &filepath, unsigned int type) {

	// read the file in one go
	std::ifstream stream(filepath);

	std::string shader;
	shader = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

	// compile the vertex shader
	unsigned int shd = CompileShader(type, shader);

	// attach the new shader to the existing porgram
	GLCall(glAttachShader(m_RendererID, shd));

	// link the entire program (locate uniform / link the ins and outs i think)
	GLCall(glLinkProgram(m_RendererID));
	GLCall(glValidateProgram(m_RendererID));

	GLCall(glDeleteShader(shd));
}

void Shader::SetUniform1i(const std::string name, int value) {
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string name, float v0, float v1, float v2) {
	GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
}
void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}
void Shader::SetUniformMat4f(const std::string name, const glm::mat4 &matrix) {
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

// the uniform location and cache it
int Shader::GetUniformLocation(const std::string &name) {

	// check if already computed
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	// if not cached ask openGL
	GLCall(int loc = glGetUniformLocation(m_RendererID, name.c_str()));
	if (loc == -1) {
		std::cout << "[Shader]: Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
	}

	// add to cache
	m_UniformLocationCache[name] = loc;

	return loc;
}

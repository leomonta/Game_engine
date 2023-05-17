/**
* Docs: https://docs.gl
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "Debugging.h"
#include "renderer.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

//#define FULLSCREEN

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

glm::vec2 translation(0, 0);
glm::mat4 proj;
glm::mat4 MVP;
glm::mat4 view;
glm::mat4 model;

void print_matrix(glm::mat4 matrix) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("% 4.2f ", matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_REPEAT) {
		if (key == GLFW_KEY_RIGHT) {
			translation.x -= 5;
		} else if (key == GLFW_KEY_LEFT) {
			translation.x += 5;
		}
		if (key == GLFW_KEY_UP) {
			translation.y -= 5;
		} else if (key == GLFW_KEY_DOWN) {
			translation.y += 5;
		}
		view = glm::translate(glm::mat4(1.0f), glm::vec3(-translation.x, translation.y, 0));
		MVP = proj * view;
		// print_matrix(MVP);
	}
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	proj = glm::ortho(0.0f, float(width), float(height), 0.0f, -1.0f, 1.0f);

	std::cout << width << " : " << height << std::endl;

	print_matrix(proj);

	std::cout << std::endl;

	glm::vec4 temp(300.0f, 300.0f, 0.0f, 1.0f);
	glm::vec4 res = temp * proj;
	for (int i = 0; i < 4; i++) {
		printf("%5.08f\n", res[i]);
	}
}

int main(void) {
	GLFWwindow *window;
	// Initialize the library
	if (!glfwInit())
		return -1;

// Create a windowed mode window and its OpenGL context
#ifdef FULLSCREEN
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "WOOOO HOOOO", NULL, NULL);
#endif

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << "Error calling Glewinit";
		return -1;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float pos[] = {
			// vertex x, vertx y, textcoor x, textcoor y
			100.0f, 300.0f, 0.0f, 0.0f, // 0 down left
			300.0f, 300.0f, 1.0f, 0.0f, // 1 down right
			300.0f, 100.0f, 1.0f, 1.0f, // 2 up right
			100.0f, 100.0f, 0.0f, 1.0f, // 3 up left
			0.0f, 0.0f, 0.5f, 0.5f		// 4 center
		};

		Renderer renderer;

		// enabling aplha
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA));

		VertexArray	 va;
		VertexBuffer vb(pos, 5 * 4 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(2, GL_FLOAT);
		layout.Push(2, GL_FLOAT);
		va.AddBuffer(vb, layout);

		// using the vertices to create a square
		unsigned int tr_i[6] = {0, 1, 2, 2, 3, 0};
		IndexBuffer	 ib(tr_i, 6);

		proj = glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
		view = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, 0));

		MVP = proj * view;

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
		shade.SetUniform1i("u_Texture", 0);	 // use the texture;

		Texture texture("res/textures/this_is_snake.jpg");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();
		Renderer::BindTexture(texture, 0);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window)) {

			// Render here
			renderer.Clear();

			Renderer::BindShaderProgram(shade);
			shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix

			Renderer::BindVertexArray(va);
			Renderer::BindIndexBuffer(ib);

			renderer.Draw(va, ib, shade);

			// Swap front and back buffers
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}

		Renderer::UnBindShaderProgram();
	}
	glfwTerminate();
	return 0;
}
/**
* Docs: https://docs.gl
*/

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>

#include "Camera.hpp"
#include "Debugging.hpp"
#include "IndexBuffer.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "profiler.hpp"

//#define FULLSCREEN

void	  printMat4(glm::mat4 mat);
void	  rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg);
void	  translateMat4(glm::mat4 &matrix, glm::vec3 amout);
glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
void	  updateDTime();
// inpu handling
void processKeyboard();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// globals
float screen_width	= 1000;
float screen_height = 1000;
bool  _Close		= false;
int	  keys[GLFW_KEY_LAST];
char  keyMod = 0x00000000b; // 0 / 0 / num lock / caps lock / super / alt / control / shift /

// MVP rojections
glm::mat4 proj(1.0f);
glm::mat4 view(1.0f);
glm::mat4 model(1.0f);
glm::mat4 MVP(1.0f);

// camera object
Camera camera(glm::vec3(0.0f, 0.5f, 2.0f));

// variable for calculating cameraposition and speed
float	  deltaTime		= 0.0f; // time elapsed since last frame
float	  lastFrameTime = 0.0f; // last absolute frametime
glm::vec2 mouse(-1.0f, -1.0f);

// lightning
glm::vec3 ambientLight(0.1f, 0.1f, 0.1f);
glm::vec3 ligthPos(2.0f, 2.0f, 2.0f);

// move camera left / right and foreward / backward
void processKeyboard() {
	glm::vec3 translation(0.0f, 0.0f, 0.0f);

	// camera controls
	// Foreward and Backward
	if (keys[GLFW_KEY_W]) {
		translation += camera.getFrontSpeed() * deltaTime;
	} else if (keys[GLFW_KEY_S]) {
		translation += camera.getFrontSpeed() * -deltaTime;
	}

	// Left and Right
	if (keys[GLFW_KEY_A]) {
		translation += camera.getRightSpeed() * -deltaTime;
	} else if (keys[GLFW_KEY_D]) {
		translation += camera.getRightSpeed() * deltaTime;
	}

	// Up and Down
	if (keys[GLFW_KEY_SPACE]) {
		translation += camera.getUpSpeed() * deltaTime;
	} else if (keys[GLFW_KEY_LEFT_SHIFT]) {
		translation += camera.getUpSpeed() * -deltaTime;
	}

	// and apply the movement
	view = camera.move(translation);

	// Zoom
	if (keys[GLFW_KEY_C]) {
		camera.zoom(-1);
		proj = camera.getPerspective(screen_width, screen_height);
	} else if (keys[GLFW_KEY_X]) {
		camera.zoom(1);
		proj = camera.getPerspective(screen_width, screen_height);
	}

	// world color
	float change = 0.05f;

	if (keyMod & GLFW_MOD_SHIFT) { // Mask other bits and check only what i'm interested int
		change *= -1;
	}
	if (keys[GLFW_KEY_I]) {
		ambientLight.r += change;
	}
	if (keys[GLFW_KEY_O]) {
		ambientLight.g += change;
	}
	if (keys[GLFW_KEY_P]) {
		ambientLight.b += change;
	}
	// close windows
	if (keys[GLFW_KEY_ESCAPE]) {
		_Close = true;
		return;
	}
}

// recalculate the perspective matrix from the new screen size and update the drawing viewport
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {

	glViewport(0, 0, width, height);
	screen_height = float(height);
	screen_width  = float(width);
	proj		  = camera.getPerspective(screen_width, screen_height);
}

// permits multiple key pressed at once
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

	keys[key] = action > 0; // _RELEASE = 0, _PRESS and _REPEAT = 1
	keyMod	  = char(mods);
}

// make the camera look to the mouse
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

	if (mouse == glm::vec2(-1.0f, -1.0f)) {
		mouse.x = float(xpos);
		mouse.y = float(ypos);
	}

	view = camera.point(float(xpos) - mouse.x, mouse.y - float(ypos));

	mouse = glm::vec2(xpos, ypos);
}

int main() {

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit())
		return -1;

// Create a windowed or fullscreen mode window and its OpenGL context
#ifdef FULLSCREEN
	monitor = glfwGetPrimaryMonitor();
	window	= glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(int(screen_width), int(screen_height), "WOOOO HOOOO", NULL, NULL);
#endif

	// if window creation failed stop everything
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// callback for window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// callback for key pressed
	glfwSetKeyCallback(window, key_callback);
	// callback for mouse positioon changed
	glfwSetCursorPosCallback(window, mouse_callback);
	// hide the mouse pointer and constrint it within the window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Make the window's context current, draw on this window
	glfwMakeContextCurrent(window);

	// set the swap interval as the v-sync
	glfwSwapInterval(1);

	// initializiong Glew
	if (glewInit() != GLEW_OK) {
		std::cout << "Error calling Glewinit";
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// enabling aplha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	// enable depth buffer
	GLCall(glEnable(GL_DEPTH_TEST));

	// scope every gl buffer, this way they will automatically destroyed
	{

		// a cube
		float pos[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // 1 right down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,   // 2 right up
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // 3 left up

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 0 left down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	  // 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 3 left up

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,   // 0 left down
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,  // 1 right down
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // 3 left up

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,	  // 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,  // 1 right down
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 2 right up
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 3 left up

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 0 left down
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,  // 1 right down
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,   // 2 right up
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  // 3 left up

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 0 left down
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1 right down
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,	  // 2 right up
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // 3 left up
		};

		Renderer renderer;

		VertexArray	 va;
		VertexBuffer vb(pos, sizeof(pos)); //5 * 24 * sizeof(float));

		VertexLayout layout;
		// 2 bytes for position, 2 for texture coordinates
		layout.Push(3, GL_FLOAT);
		layout.Push(2, GL_FLOAT);
		layout.Push(3, GL_FLOAT);
		va.AddBuffer(vb, layout);

		// using the vertices to create a square
		unsigned int tr_i[36] = {
			0, 1, 2, 2, 3, 0,		// square one
			4, 5, 6, 6, 7, 4,		// square two
			8, 9, 10, 10, 11, 8,	// square three
			12, 13, 14, 14, 15, 12, // square four
			16, 17, 18, 18, 19, 16, // square five
			20, 21, 22, 22, 23, 20	// square six
		};
		IndexBuffer ib(tr_i, 36);

		proj = camera.getPerspective(screen_width, screen_width);
		view = camera.point(0, -1);

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniform1i("u_Texture", 0); // use the texture;
		shade.SetUniform3f("u_LightPos", ligthPos.x, ligthPos.y, ligthPos.z);
		shade.SetUniform3f("u_lightColor", 1.0f, 1.0f, 2.0f);

		Texture texture("res/textures/this_is_snake.jpg");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		// bind the used stuff
		Renderer::BindTexture(texture, 0);
		Renderer::BindVertexArray(va);
		Renderer::BindIndexBuffer(ib);
		Renderer::BindShaderProgram(shade);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window) && !_Close) {

			processKeyboard();
			// update delta time for camera movement
			updateDTime();

			// Render here
			renderer.Clear();

			MVP = proj * view * model;
			shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
			shade.SetUniformMat4f("u_Model", model);
			shade.SetUniform3f("u_ambientLight", ambientLight.r, ambientLight.g, ambientLight.b);

			renderer.Draw(va, ib, shade);

			// Swap front and back buffers
			// show front buffer, work on back buffer
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();
	}
	glfwTerminate();
	return 0;
}

// for debug, print the matrix on console
void printMat4(glm::mat4 matrix) {

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("% 4.8f ", matrix[i][j]);
		}
		std::cout << std::endl;
	}
}

void translateMat4(glm::mat4 &matrix, glm::vec3 amount) {
	matrix = glm::translate(matrix, glm::vec3(amount.x, amount.y, amount.z));
}

void rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg) {
	matrix = glm::rotate(matrix, glm::radians(amountDeg.x), glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(amountDeg.y), glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, glm::radians(amountDeg.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 x = v2 - v1;
	std::cout << x.x << " : " << x.y << " : " << x.z << std::endl;
	glm::vec3 y = v3 - v1;
	std::cout << y.x << " : " << y.y << " : " << y.z << std::endl;

	glm::vec3 res = glm::cross(x, y);
	std::cout << res.x << " : " << res.y << " : " << res.z << std::endl;
	return res;
}

void updateDTime() {

	// framerate dependent way of measuring time
	float currentFrame = float(glfwGetTime());
	deltaTime		   = currentFrame - lastFrameTime;
	lastFrameTime	   = currentFrame;
}
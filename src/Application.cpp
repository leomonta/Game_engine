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

// function declaration
void	  printMat4(glm::mat4 mat);
void	  rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg);
void	  translateMat4(glm::mat4 &matrix, glm::vec3 amout);
glm::vec3 calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
void	  updateDTime();
bool	  intersect_triangle(glm::vec3 Raydir, glm::vec3 Rayorg, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 &res); // calculate the intersect pont
void	  checkMouseRay();

// input handling
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
glm::vec3 ambientLight(1.5f, 1.5f, 1.5f);
glm::vec3 ligthPos(2.0f, 2.0f, 2.0f);

// vertexes buffer and index buffer

// using the vertices to create trises
unsigned int indexes[MAX_VERTEX_COUNT]	= {};
Vertex		 vertexes[MAX_VERTEX_COUNT] = {};
int			 cube_Index					= 0;
Vertex		 cube[24]					= {
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f}, // 0 left down
	{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 2 right up
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 0 left down
	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},		// 2 right up
	{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 0 left down
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f}, // 2 right up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 3 left up
	{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},		// 0 left down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 2 right up
	{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f}, // 0 left down
	{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 2 right up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 0 left down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f},		// 2 right up
	{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 0.0f}		// 3 left up
};

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

	if (keys[GLFW_KEY_K]) {
		if (24 * cube_Index + 23 < MAX_VERTEX_COUNT) {

			for (int i = 0; i < 24; i++) {
				vertexes[i + (24 * cube_Index)] = cube[i];
				cube[i].Vert_position.x += 1.0f;
			}

			cube_Index++;
		}

		keys[GLFW_KEY_K] = 0;
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

// find the intersection point between a line and a tris
bool intersect_triangle(glm::vec3 Raydir, glm::vec3 Rayorg, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 &res) {

	glm::vec3 E1 = B - A; // Vector pointing from the "origin" vertex to another one
	glm::vec3 E2 = C - A; // Vector pointing from the "origin" vertex to another one

	glm::vec3 N	  = glm::cross(E1, E2);
	float	  det = -glm::dot(Raydir, N);

	float invdet = 1.0f / det;

	glm::vec3 AO  = Rayorg - A; // Vector from the "origin" of the triangle to the origin of the line
	glm::vec3 DAO = glm::cross(AO, Raydir);
	float	  u	  = glm::dot(E2, DAO) * invdet;
	float	  v	  = -glm::dot(E1, DAO) * invdet;
	float	  t	  = glm::dot(AO, N) * invdet;
	if (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0) {
		res = Rayorg + (t * Raydir);
		return true;
	}
	return false;
}

void checkMouseRay() {

	glm::vec3 pos;
	glm::vec3 A, B, C;

	for (int i = 0; i < MAX_VERTEX_COUNT - 3; i += 3) {
		A = vertexes[indexes[i + 0]].Vert_position;
		B = vertexes[indexes[i + 1]].Vert_position;
		C = vertexes[indexes[i + 2]].Vert_position;
		intersect_triangle(camera.m_cameraWatching, camera.m_cameraPosition, A, B, C, pos);
		if (pos.x != 0.0f && pos.y != 0.0f && pos.z != 0.0f) {
			std::cout << pos.x << " : " << pos.y << " : " << pos.z << std::endl;
			pos = {0.0f, 0.0f, 0.0f};
		}
	}
}
/*
bool RayIntersectsTriangle(glm::vec3 rayOrigin, glm::vec3 rayDir, glm::vec3 *inTriangle, glm::vec3 &outIntersectionPoint) {
	const float EPSILON = 0.0000001;
	glm::vec3	vertex0 = inTriangle[0];
	glm::vec3	vertex1 = inTriangle[1];
	glm::vec3	vertex2 = inTriangle[2];
	glm::vec3	edge1, edge2, h, s, q;
	float		Determinant, InverseDeterminant, u, v; // Determinant is also the Angle of the line relative to the tris

	edge1 = vertex1 - vertex0; // Vector pointing from the "origin" vertex to another one
	edge2 = vertex2 - vertex0; // Vector pointing from the "origin" vertex to another one

	h			= glm::cross(rayDir, edge2);
	Determinant = glm::dot(edge1, h);
	if (Determinant > -EPSILON && Determinant < EPSILON) { // -0.0001 < Determinant < 0.001
		return false; // This ray is parallel to this triangle.
	}

	InverseDeterminant = 1.0 / Determinant;

	s = rayOrigin - vertex0; // Vector from the "origin" of the triangle to the origin of the line
	u = InverseDeterminant * glm::dot(s, h);

	if (u < 0.0 || u > 1.0) {
		return false;
	}

	q = glm::cross(s, edge1);
	v = InverseDeterminant * glm::dot(rayDir, q);

	if (v < 0.0 || u + v > 1.0) {
		return false;
	}

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		outIntersectionPoint = rayOrigin + rayDir * t;
		return true;
	} else // This means that there is a line intersection but not a ray intersection.
		return false;
}
*/
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

		Renderer renderer;

		VertexArray	 va;
		VertexBuffer vb(sizeof(vertexes)); // 10 cubes

		va.AddBuffer(vb);

		int offset = 0;
		for (int i = 0; i < 36 * 10; i += 6) {
			indexes[i + 0] = 0 + offset;
			indexes[i + 1] = 1 + offset;
			indexes[i + 2] = 2 + offset;
			indexes[i + 3] = 2 + offset;
			indexes[i + 4] = 3 + offset;
			indexes[i + 5] = 0 + offset;

			offset += 4;
		}
		IndexBuffer ib(indexes, MAX_VERTEX_COUNT);

		proj = camera.getPerspective(screen_width, screen_width);
		view = camera.point(0, -1);

		Shader shade;
		shade.addShader("res/shaders/basic.frag", GL_FRAGMENT_SHADER);
		shade.addShader("res/shaders/basic.vert", GL_VERTEX_SHADER);

		Renderer::BindShaderProgram(shade);
		shade.SetUniform1i("u_Texture", 0); // use the texture;

		Texture redlamp_on("res/textures/redstone_lamp_on.png");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		// bind the used stuff
		Renderer::BindTexture(redlamp_on, 0);
		Renderer::BindVertexArray(va);
		Renderer::BindIndexBuffer(ib);
		Renderer::BindShaderProgram(shade);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window) && !_Close) {

			processKeyboard();
			checkMouseRay();

			// update delta time for camera movement
			updateDTime();

			vb.submitData(&vertexes);

			// Render here
			renderer.Clear();

			MVP = proj * view * model;
			shade.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
			shade.SetUniformMat4f("u_Model", model);
			shade.SetUniform3f("u_LightColor", ambientLight.r, ambientLight.g, ambientLight.b);
			shade.SetUniform3f("u_LightPos", ligthPos.x, ligthPos.y, ligthPos.z);
			shade.SetUniform3f("u_CameraPos", camera.m_cameraPosition.x, camera.m_cameraPosition.y, camera.m_cameraPosition.z);

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
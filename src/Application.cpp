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
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "profiler.hpp"

//#define FULLSCREEN

// function declaration
GLFWwindow *setuo();
void		rotateMat4(glm::mat4 &matrix, glm::vec3 amountDeg);
void		translateMat4(glm::mat4 &matrix, glm::vec3 amout);
void		debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
glm::vec3	calcNormal(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
void		updateDTime();
bool		intersect_triangle(glm::vec3 Raydir, glm::vec3 Rayorg, glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 &res); // calculate the intersect point to any tris closest to the where the camera is watching
glm::vec3	checkMouseRay(Vertex *vertexes, unsigned int *indexes, unsigned int count);

// input handling
void processKeyboard();
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

// Drawing utils
void addCube(Vertex *cube, Renderer &renderer);
void shift(Vertex *verticies, glm::vec3 offset, int count);

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
Camera camera(glm::vec3(5.0f, 5.5f, 2.0f));

// variable for calculating cameraposition and speed
float	  deltaTime		= 0.0f; // time elapsed since last frame
float	  lastFrameTime = 0.0f; // last absolute frametime
glm::vec2 mouse(-1.0f, -1.0f);

// lightning
glm::vec3 ambientLight(1.5f, 1.5f, 1.5f);
glm::vec3 ligthPos(2.0f, 2.0f, 2.0f);

// vertexes buffer and index buffer

bool chunk[10][10]; // true: a block is present, false: a block is not present

int cube_bases[10] = {0};
// using the vertices to create trises
Vertex cube[24] = {
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}, // 0 left down
	{{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 2 right up
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 0 left down
	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},		// 2 right up
	{{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 0 left down
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}, // 2 right up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
	{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},		// 0 left down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 2 right up
	{{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}, // 0 left down
	{{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 2 right up
	{{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
	{{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 0 left down
	{{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},		// 2 right up
	{{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}		// 3 left up
};

// the small crosshair quad
Vertex croshair_verts[4]{

	{{-0.03f, -0.03f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f}, // 0 left down
	{{0.03f, -0.03f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 1 right down
	{{0.03f, 0.03f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 2 right up
	{{-0.03f, 0.03f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f},	// 3 left up
};

GLFWwindow *setup() {

	GLFWwindow *window;

	// Initialize the library
	if (!glfwInit()) {
		std::cout << "[Error]: GLFW initialization failed" << std::endl;
		return nullptr;
	}

	std::cout << "[INFO]: GLFW successfully initialized" << std::endl;

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

// Create a windowed or fullscreen mode window and its OpenGL context
#ifdef FULLSCREEN
	monitor = glfwGetPrimaryMonitor();
	window	= glfwCreateWindow(1920, 1080, "WOOOO HOOOO", monitor, NULL);
#else
	window = glfwCreateWindow(int(screen_width), int(screen_height), "WOOOO HOOOO", NULL, NULL);
#endif

	// if window creation failed stop everything
	if (!window) {
		std::cout << "[ERROR]: Window creation failed" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	std::cout << "[INFO]: window created successfully" << std::endl;

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
		std::cout << "[ERROR]: GLEW initialization failed" << std::endl;
		return nullptr;
	}

	std::cout << "[INFO]: GLEW successfully initialized" << std::endl;

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)(debugMessage), NULL);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

	std::cout << "[INFO]: OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// enabling aplha
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	// enable depth buffer
	GLCall(glEnable(GL_DEPTH_TEST));

	return window;
}

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

	// [DEBUG]: add a cube
	/*if (keys[GLFW_KEY_K]) {

		cube_Index += 4;
		keys[GLFW_KEY_K] = 0;
	}*/
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

	const float EPSILON = 0.0000001f;
	glm::vec3	edge1, edge2, h, AO, q;
	float		det, invDet, u, v;
	edge1 = B - A;
	edge2 = C - A;
	h	  = glm::cross(Raydir, edge2);
	det	  = glm::dot(edge1, h);
	if (det > -EPSILON && det < EPSILON)
		return false; // This ray is parallel to this triangle.

	invDet = 1.0f / det;
	AO	   = Rayorg - A;
	u	   = invDet * glm::dot(AO, h);
	if (u < 0.0 || u > 1.0)
		return false;

	q = glm::cross(AO, edge1);
	v = invDet * glm::dot(Raydir, q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = invDet * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
	{
		res = Rayorg + (Raydir * t);
		return true;
	} else // This means that there is a line intersection but not a ray intersection.
		return false;
}

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity: notification";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

glm::vec3 checkMouseRay(Vertex *vertexes, unsigned int *indexes, unsigned int count) {

	glm::vec3 pos, min = {0.0f, 0.0f, 0.0f};
	glm::vec3 A, B, C;

	for (unsigned int i = 0; i < count - 3; i += 3) {
		A = vertexes[indexes[i + 0]].Vert_position;
		B = vertexes[indexes[i + 1]].Vert_position;
		C = vertexes[indexes[i + 2]].Vert_position;
		intersect_triangle(camera.m_cameraWatching, camera.m_cameraPosition, A, B, C, pos);

		if (glm::length(min) <= 0.000001) {
			min = pos;
		}

		float a = glm::length(pos - camera.m_cameraPosition);
		float b = glm::length(min - camera.m_cameraPosition);
		if (a < b) {
			min = pos;
		}
	}

	return min;
}

void addCube(Vertex *verticies, Renderer &renderer) {

	for (int i = 0; i < 24; i += 4) {
		renderer.DrawQuad(verticies[i + 0], verticies[i + 1], verticies[i + 2], verticies[i + 3]);
	}
}

void shift(Vertex *verticies, glm::vec3 offset, int count) {
	for (int i = 0; i < count; i++) {
		verticies[i].Vert_position += offset;
	}
}

int main() {

	// creating OpenGL context and window
	GLFWwindow *window = setup();

	// scope every gl buffer, this way they will automatically destroyed
	{
		chunk[5][5] = true;

		proj = camera.getPerspective(screen_width, screen_width);
		view = camera.point(0, -1);

		//  --- GAME ---

		Shader Game_shader;
		Game_shader.addShader("res/shaders/main.vert", GL_VERTEX_SHADER);
		Game_shader.addShader("res/shaders/main.frag", GL_FRAGMENT_SHADER);

		Renderer::BindShaderProgram(Game_shader.m_RendererID);
		Game_shader.SetUniform1i("u_Texture", 0); // use the texture;

		Texture redlamp_on("res/textures/redstone_lamp_on.png");

		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		Renderer Game_renderer;

		//  --- HUD ---

		Shader HUD_shader;
		HUD_shader.addShader("res/shaders/hud.vert", GL_VERTEX_SHADER);
		HUD_shader.addShader("res/shaders/hud.frag", GL_FRAGMENT_SHADER);

		Renderer::BindShaderProgram(HUD_shader.m_RendererID);
		HUD_shader.SetUniform1i("u_Texture", 1);

		Texture CrossHair("res/textures/CrossHair.png");

		Renderer HUD_renderer;

		// Unbind everything to be sure
		Renderer::UnBindVertexArray();
		Renderer::UnBindVertexBuffer();
		Renderer::UnBindIndexBuffer();
		Renderer::UnBindShaderProgram();

		// bind the used stuff
		Renderer::BindTexture(redlamp_on.m_RendererID, 0);
		Renderer::BindTexture(CrossHair.m_RendererID, 1);

		// Main game loop
		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window) && !_Close) {

			processKeyboard();

			// update delta time for camera movement
			updateDTime();

			// Render here
			Game_renderer.Clear();

			{ // GAME
				Renderer::BindShaderProgram(Game_shader.m_RendererID);

				MVP = proj * view * model;
				Game_shader.SetUniformMat4f("u_MVP", MVP); // use the projection matrix
				Game_shader.SetUniformMat4f("u_Model", model);
				Game_shader.SetUniform3f("u_LightColor", ambientLight.r, ambientLight.g, ambientLight.b);
				Game_shader.SetUniform3f("u_LightPos", ligthPos.x, ligthPos.y, ligthPos.z);
				Game_shader.SetUniform3f("u_CameraPos", camera.m_cameraPosition.x, camera.m_cameraPosition.y, camera.m_cameraPosition.z);

				for (int i = 0; i < 10; i++) {
					for (int j = 0; j < 10; j++) {
						if (chunk[i][j]) {
							shift(cube, {i, j, 0}, 24);
							addCube(cube, Game_renderer);
							shift(cube, {-i, -j, 0}, 24);
						}
					}
				}

				if (keys[GLFW_KEY_ENTER]) {

					glm::vec3 base = checkMouseRay(Game_renderer.Current_batch.VertBuffer, Game_renderer.Current_batch.IndxBuffer, Game_renderer.Current_batch.indexCount);

					int I = int(roundf(base.x));
					int J = int(roundf(base.y));

					if (chunk[I][J]) {

						I = int(floorf(base.x));
						J = int(floorf(base.y));
					}

					chunk[I][J] = true;

					keys[GLFW_KEY_ENTER] = 0;
				}

				// there is a breakpoint here, useful for quick debug stop
				if (keys[GLFW_KEY_K]) {
					keys[GLFW_KEY_K] = 0;
				}

				Game_renderer.Commit();

			} // END GAME

			{ // HUD
				Renderer::BindShaderProgram(HUD_shader.m_RendererID);

				HUD_renderer.DrawQuad(croshair_verts[0], croshair_verts[1], croshair_verts[2], croshair_verts[3]);

				HUD_renderer.Commit();

			} // END HUD

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
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <playground/camera.h>
#include <playground/model.h>

#include <common/shader.hpp>

#include <iostream>

/// <summary>
///		GLFW Window
/// </summary>
GLFWwindow* window;

/// <summary>
///		Width and height of the window
/// </summary>
const unsigned int SCR_WIDTH = 1920, SCR_HEIGHT = 1080;

/// <summary>
///		Initial camera position
/// </summary>
Camera camera = Camera(glm::vec3(0.0f, 1.0f, 3.0));

/// <summary>
///		Initial mouse position
/// </summary>
float lastX = SCR_WIDTH / 2.0f, lastY = SCR_HEIGHT / 2.0f;

/// <summary>
///		For mouse movement at the beginning of the scene
/// </summary>
bool firstMouse = true;

/// <summary>
///		Passed time since last frame
/// </summary>
float deltaTime = 0.0f;

/// <summary>
///		Timestamp of last frame
/// </summary>
float lastFrame = 0.0f;

/// <summary>
///		Total time passed
/// </summary>
float totalTimePassed = 0.0f;

/// <summary>
///		VAO, VBO of the cubes
/// </summary>
unsigned int cubeVBO, cubeVAO;

/// <summary>
///		Template, vertices for a cube (adjust with scaling as necessary)
/// </summary>
float vertices[] = {
	// positions          // normals           // texture coords
	// BACK
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // bottom left
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // bottom right
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // top right

	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // top right
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // top left
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // bottom left

	// FRONT
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom left
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, // bottom right
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // top right

	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // top right
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, // top left
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // bottom left

	// LEFT
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top right
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top left
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom right

	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom left
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // bottom right
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top right

	// RIGHT
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top left
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top right
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom right

	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom right
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // bottom left
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top left

	 // BOTTOM
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,

	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	// TOP
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

/// <summary>
///		Positions, where to place cubes
/// </summary>
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
};

/// <summary>
///		Animation Loop
/// </summary>
void update();

/// <summary>
///		Update attributes of lighting shader
/// </summary>
/// <param name="lightingShader">Lighting Shader to be updated</param>
void updateLightingShaderInformation(Shader& lightingShader, glm::mat4& model, glm::mat4& view, glm::mat4& projection);

/// <summary>
///		Process user input
/// </summary>
/// <param name="window">Window</param>
void processInput(GLFWwindow* window);

/// <summary>
///		Executed when window size is changed
/// </summary>
/// <param name="window">Window</param>
/// <param name="width">Viewport width</param>
/// <param name="height">Viewport height</param>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/// <summary>
///		Executed when mouse is moved
/// </summary>
/// <param name="window">Window</param>
/// <param name="xpos">Mouse x position</param>
/// <param name="ypos">Mouse y position</param>
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

/// <summary>
///		Executed when mouse wheel is used
/// </summary>
/// <param name="window">Window</param>
/// <param name="xoffset">x offset</param>
/// <param name="yoffset">y offset</param>
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

/// <summary>
///		Utility function, load 2D textures from a file
/// </summary>
/// <param name="path">Path to the texture</param>
/// <returns>Texture ID</returns>
unsigned int loadTexture(const char* path);

/// <summary>
///		Initialize GLFW window
/// </summary>
/// <returns></returns>
GLFWwindow* initializeWindow();

/// <summary>
///		Load OpenGL function pointers (GLAD)
/// </summary>
void initializeFunctionPointers();
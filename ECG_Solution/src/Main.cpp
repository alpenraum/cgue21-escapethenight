#include "utils/Utils.h"
#include <sstream>
#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include "Texture.h"
#include <iostream>
#include "Model.h"
#include <stdio.h>
#include "ICamera.h"
#include "BasicCamera.h"
#include "Skybox.h"
#include "WaterFrameBuffer.h"
#include "Watertile.h"
#include "WaterRenderer.h"
#include "utils/Settings.h"
#include "WorldRenderer.h"
#include "PxPhysicsAPI.h"
#include "Converter.h"
#include "PhysXScene.h"

using namespace physx;
#pragma warning( disable : 4244 )


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderQuad();
/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

BasicCamera camera;

static bool _wireframe = false;
static bool _culling = true;
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 6.0f;
static float gamma;
static bool isFPCamera = false;
static bool NORMALMAPPING = true;
static bool won = false;

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;
static PxFoundation* gFoundation = NULL;

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	std::cout << "STARTING" << std::endl;
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	INIReader reader("assets/settings.ini");
	Settings::loadSettings(reader);
	std::string window_title = reader.Get("window", "title", "ECG");

	/* --------------------------------------------- */
	// Create context
	/* --------------------------------------------- */

	if (!glfwInit()) {
		EXIT_WITH_ERROR("Failed to init GLFW")
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Request OpenGL version 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Request core profile
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  // Create an OpenGL debug context
	glfwWindowHint(GLFW_REFRESH_RATE, Settings::refreshRate); // Set refresh rate
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	// Enable antialiasing (4xMSAA)
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open window
	GLFWmonitor* monitor = nullptr;

	if (Settings::fullscreen)
		monitor = glfwGetPrimaryMonitor();

	GLFWwindow* window = glfwCreateWindow(Settings::width, Settings::height, window_title.c_str(), monitor, nullptr);

	if (!window) {
		glfwTerminate();
		EXIT_WITH_ERROR("Failed to create window")
	}

	// This function makes the context of the specified window current on the calling thread.
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true;
	GLenum err = glewInit();

	// If GLEW wasn't initialized
	if (err != GLEW_OK) {
		EXIT_WITH_ERROR("Failed to init GLEW")
	}

	// Debug callback
	if (glDebugMessageCallback != NULL) {
		// Register your callback function.

		glDebugMessageCallback(DebugCallbackDefault, NULL);
		// Enable synchronous callback. This ensures that your callback function is called
		// right after an error has occurred. This capability is not defined in the AMD
		// version.
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	/* --------------------------------------------- */
	// Init framework
	/* --------------------------------------------- */

	if (!initFramework()) {
		EXIT_WITH_ERROR("Failed to init framework")
	}

	// set callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//GLFW Configurations
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set GL defaults
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable CLipping planes
	glEnable(GL_CLIP_DISTANCE0);

	std::cout << "SCENE INIT" << std::endl;
	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

		// Load shader(s)
	std::shared_ptr<Shader> textShader = std::make_shared<Shader>("text.vert", "text.frag");

	// Initialize camera
	//Camera camera(fov, float(window_width) / float(window_height), nearZ, farZ);
	camera = BasicCamera(Settings::fov, ((double)Settings::width / (double)Settings::height), Settings::nearPlane, Settings::farPlane, Settings::mouseSens);

	/* --------------------------------------------- */
	// Models
	/* --------------------------------------------- */

		//SKYBOX
	std::vector<std::string> skyboxFaces
	{
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
	};

	std::vector<Model*> modelList = std::vector<Model*>();
	std::vector<Watertile*> watertiles = std::vector<Watertile*>();
	//Model renderObject = Model("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(0.0f, 5.0f, -10.0f));
	//Model betweenWaterBird = Model("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(0.0f, -10.0f, -10.0f));
	//Model subWaterBird = Model("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(17.0f, 5.0f, 10.0f));
	Model terrain = Model("assets/models/LowPolyMountains_obj/lowpolymountains.obj", glm::vec3(0.0f, 0.5f, 0.0f));
	modelList.push_back(&terrain);
	//modelList.push_back(&renderObject);
	//modelList.push_back(&subWaterBird);
	//modelList.push_back(&betweenWaterBird);

	//Watertile watertile = Watertile(glm::vec3(-15.0f, 0.0f, 14.0f), glm::vec2(10.0f), 0.03f);
	//watertiles.push_back(&watertile);
	//Watertile tile1 = Watertile(glm::vec3(15.0f, 0.0f, 7.0f), glm::vec2(10.0f), 0.1f);
	//watertiles.push_back(&tile1);

	/* --------------------------------------------- */
	// Lights
	/* --------------------------------------------- */
	std::vector<DirectionalLight*> dirLights = std::vector<DirectionalLight*>();
	std::vector<PointLight*> pointLights = std::vector<PointLight*>();

	//for good attentuation values, see: http://wiki.ogre3d.org/-Point+Light+Attenuation

	PointLight sun = PointLight(glm::normalize(glm::vec3(0.6f, 0.5f, 0.3f)) * 4.0f, glm::vec3(30.0f, 10.0f, 15.0f), glm::vec3(1.0f, 0.045f, 0.0075f));
	sun.toggleShadows();
	pointLights.push_back(&sun);
	for (int i = 0; i < 3; i++) {
		PointLight* pointL = new PointLight(glm::normalize(glm::vec3(1.0f)) * 4.0f, glm::vec3((-20.0f) + 20.0f * i, 20.0f, 0), glm::vec3(1.0f, 0.045f, 0.0075f));
		pointL->toggleShadows();
		pointLights.push_back(pointL);
	}

	/* --------------------------------------------- */
	// Renderers
	/* --------------------------------------------- */
	WorldRenderer worldRenderer = WorldRenderer(modelList, watertiles, skyboxFaces, &dirLights, &pointLights);

	// Render loop
	float t = float(glfwGetTime());
	float dt = 0.0f;
	float t_sum = 0.0f;
	double mouseX = Settings::width / 2.0, mouseY = Settings::height / 2.0;
	float oldX = mouseX, oldY = mouseY;
	glm::vec2 mouseDelta = glm::vec2(0.0f);
	bool w, a, s, d, shift, space, e;

	//inint physx
	initPhysics;

	std::cout << "Scene Loaded" << std::endl;
	while (!glfwWindowShouldClose(window)) {
		//RENDERING

		// Clear backbuffer

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Poll events
		glfwPollEvents();

		// Read polling inputs
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//glfwSetCursorPos(window, window_width / 2.0, window_height / 2.0);
		w = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
		a = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
		s = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
		d = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
		shift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
		space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		e = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

		//calulate Mouse
		mouseDelta.x = mouseX - oldX;
		mouseDelta.y = mouseY - oldY;
		oldX = mouseX;
		oldY = mouseY;

		//_-------------------------------------------------------------------------------_
		//TODO ADD CHARACTER CONTROLLER WHICH PARSES WASD SPACE SHIFT TO PLAYER AND CAMERA
		unsigned int direction = ICamera::NO_MOVEMENT;
		if (w) {
			direction += ICamera::FORWARD;
		}
		else if (s) {
			direction += ICamera::BACKWARD;
		}

		if (a) {
			direction += ICamera::LEFT;
		}
		else if (d) {
			direction += ICamera::RIGHT;
		}

		camera.ProcessKeyboard(direction, dt, shift);
		camera.ProcessMouseMovement(mouseDelta, dt);
		camera.updateCamera();

		//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		worldRenderer.render(&camera, dt, false, NORMALMAPPING);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Compute frame time
		dt = t;
		t = float(glfwGetTime());
		dt = t - dt;
		t_sum += dt;

		// Swap buffers
		glfwSwapBuffers(window);
	}

	/* --------------------------------------------- */
	// Destroy framework
	/* --------------------------------------------- */

	destroyFramework();
	worldRenderer.cleanUp();

	/* --------------------------------------------- */
	// Destroy context and exit
	/* --------------------------------------------- */

	glfwTerminate();

	return EXIT_SUCCESS;
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		_dragging = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		_strafing = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		_strafing = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	_zoom -= float(yoffset) * 0.5f;
}

/**
 * Callback function for keypresses and releases that require no polling.
 */
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Keyup Events
	// F1 - Wireframe
	// F2 - Culling
	// F3 - Show/Hide HUD permanently
	// Esc - Exit

	// Keydown Events
	// Space - Jump

	if (action == GLFW_KEY_DOWN) {
		switch (key)
		{
		case GLFW_KEY_SPACE:

			break;

		case GLFW_KEY_F:

			break;
		case GLFW_KEY_1:

			break;
		case GLFW_KEY_2:

			break;
		case GLFW_KEY_3:

			break;
		case GLFW_KEY_BACKSPACE:

			break;
		}
	}
	else if (action != GLFW_RELEASE) return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	case GLFW_KEY_F1:
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
		break;
	case GLFW_KEY_F2:
		_culling = !_culling;
		if (_culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
		break;
	case GLFW_KEY_F3:
		NORMALMAPPING = !NORMALMAPPING;
		break;
	case GLFW_KEY_F4:
		LOG_TO_CONSOLE("posX:", camera.getPosition().x);
		LOG_TO_CONSOLE("posY:", camera.getPosition().y);
		LOG_TO_CONSOLE("posZ:", camera.getPosition().z);
		break;
	case GLFW_KEY_F6:

		break;
	}
}

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	// The AMD variant of this extension provides a less detailed classification of the error,
	// which is why some arguments might be "Unknown".
	switch (source) {
	case GL_DEBUG_CATEGORY_API_ERROR_AMD:
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_CATEGORY_APPLICATION_AMD:
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD:
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD:
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_CATEGORY_DEPRECATION_AMD:
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD:
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_CATEGORY_PERFORMANCE_AMD:
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_CATEGORY_OTHER_AMD:
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}

/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#include "utils/Utils.h"
#include <sstream>
#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include <iostream>
#include "Model.h"
#include <stdio.h>
#include "ICamera.h"
#include "BasicCamera.h"
#include "Skybox.h"
#include "Watertile.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffer.h"
#include "utils/Settings.h"
#include "WorldRenderer.h"
#include "TwoDGridPF.h"
#include "Pathfinder.h"
#include "Player.h"
#include "Killer.h"
#include "FireParticleSystem.h"
#include "utils/Time.h"
#include "PhysxMaster.h"
#include "StaticModel.h"
#include "DynamicModel.h"
#include "AnimatedModel.h"
#pragma warning( disable : 4244 )

using namespace physx;
/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderQuad();
void showEndScreen();

void printControls();

/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

BasicCamera* camera;
Player* player;
Killer* killer;

static bool _wireframe = false;
static bool _culling = true;
static float _zoom = 6.0f;
static float gamma;
static bool isFPCamera = false;
static bool NORMALMAPPING = true;
static bool won = false;

static bool usePlayerCamera = true;

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

	std::cout << "PHYSX INIT" << std::endl;
	PhysxMaster physxMaster = PhysxMaster();


	std::cout << "SCENE INIT" << std::endl;
	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */

		// Load shader(s)
	std::shared_ptr<Shader> textShader = std::make_shared<Shader>("text.vert", "text.frag");

	/* --------------------------------------------- */
	// Player, Camera, other Entities
	/* --------------------------------------------- */
	//Camera camera(fov, float(window_width) / float(window_height), nearZ, farZ);
	BasicCamera freeCamera = BasicCamera(Settings::fov, ((double)Settings::width / (double)Settings::height), Settings::nearPlane, Settings::farPlane, Settings::mouseSens);

	player = new Player(glm::vec3(0.0f,30.0f,0.0f),&physxMaster);

	killer = new Killer(glm::vec3(5.0f,5.0f,0.0f),&physxMaster);





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

	Model renderObject = Model("assets/models/killer/killer.dae", glm::vec3(0.0f, 2.0f, -10.0f));
	Model betweenWaterBird = Model("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(0.0f, -10.0f, -10.0f));
	StaticModel terrain = StaticModel("assets/models/LowPolyMountains_obj/lowpolymountains.obj", glm::vec3(0.0f, 0.5f, 0.0f),&physxMaster,true);

	// SCALE NOT WORKING ATM, NOT FOR PHYSX AT LEAST
	DynamicModel shadowBird1 = DynamicModel("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(2.0f, 10.0f, 12.0f), glm::vec3(0.2f), &physxMaster, false);
	shadowBird1.setMass(15.0f);
	DynamicModel shadowBird2 = DynamicModel("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(-2.0f, 7.0f, 12.0f), glm::vec3(0.2f), &physxMaster, false);
	shadowBird2.setMass(25);
	DynamicModel shadowBird3 = DynamicModel("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(2.0f, 5.0f, 8.0f), glm::vec3(0.2f), &physxMaster, false);
	shadowBird3.setMass(25.0f);
	DynamicModel shadowBird4 = DynamicModel("assets/models/bullfinch_obj/bullfinch.obj", glm::vec3(-2.0f, 3.0f, 8.0f), glm::vec3(0.2f), &physxMaster, false);
	shadowBird4.setMass(25);

	modelList.push_back(&renderObject);
	modelList.push_back(&shadowBird1);
	modelList.push_back(&shadowBird2);
	modelList.push_back(&shadowBird3);
	modelList.push_back(&shadowBird4);
	modelList.push_back(&betweenWaterBird);

	Watertile watertile = Watertile(glm::vec3(-15.0f, 0.0f, 14.0f), glm::vec2(10.0f), 0.03f);
	watertiles.push_back(&watertile);
	Watertile tile1 = Watertile(glm::vec3(15.0f, 0.0f, 7.0f), glm::vec2(10.0f), 0.1f);
	watertiles.push_back(&tile1);

	modelList.push_back(&terrain);

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//TEST
	AnimatedModel am = AnimatedModel("assets/models/killer/killer.dae", glm::vec3(-2.0f, 3.0f, 8.0f), glm::vec3(1.0f));
	



	/* --------------------------------------------- */
	// Lights
	/* --------------------------------------------- */
	std::vector<DirectionalLight*> dirLights = std::vector<DirectionalLight*>();
	std::vector<PointLight*> pointLights = std::vector<PointLight*>();

	//for good attentuation values, see: http://wiki.ogre3d.org/-Point+Light+Attenuation

	PointLight moon = PointLight(glm::normalize(glm::vec3(0.517f, 0.57f, 0.8f)) * 80.0f, glm::vec3(-60.0f, 100.0f, -80.0f), glm::vec3(1.0f, 0.007f, 0.0002f));
	moon.toggleShadows();
	pointLights.push_back(&moon);


	pointLights.push_back(player->getLight());

	/* --------------------------------------------- */
	// Renderers
	/* --------------------------------------------- */
	WorldRenderer worldRenderer = WorldRenderer(modelList, watertiles, skyboxFaces, &dirLights, &pointLights);
	ParticleMaster::init();



	Time::init(glfwGetTime());

	// Render loop
	float dt;
	double mouseX = Settings::width / 2.0, mouseY = Settings::height / 2.0;
	float oldX = mouseX, oldY = mouseY;
	glm::vec2 mouseDelta = glm::vec2(0.0f);
	bool w, a, s, d, shift, space, e;

	std::cout << "Scene Loaded" << std::endl;

	printControls();
	while (!glfwWindowShouldClose(window)) {
		dt = Time::getDeltaTime();
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

		if (space) {
			player->jump();
		}




		//TODO: IMPLEMENT REAL END SCREEN
		if ((player->getSanity() <= 0.01f) || glm::distance(player->getPosition(), killer->getPosition()) <= 1.0f) {
			showEndScreen();
			dt = 0.0f; // effectively pausing the scene
		}



		if (usePlayerCamera) {
			camera = player->getCamera();
		}
		else {
			camera = &freeCamera;
		}

		physxMaster.update();

		//_-------------------------------------------------------------------------------_
		//TODO ADD CHARACTER CONTROLLER WHICH PARSES WASD SPACE SHIFT TO PLAYER AND CAMERA
		unsigned int direction = Player::NO_MOVEMENT;
		if (w) {
			direction += Player::FORWARD;
		}
		else if (s) {
			direction += Player::BACKWARD;
		}

		if (a) {
			direction += Player::LEFT;
		}
		else if (d) {
			direction += Player::RIGHT;
		}

		freeCamera.ProcessKeyboard(direction, dt, shift);
		freeCamera.ProcessMouseMovement(mouseDelta, dt);
		freeCamera.updateCamera();

		player->update(direction, mouseDelta, dt, worldRenderer.getCampfires());

		killer->update(*player, player->isNearLight(worldRenderer.getCampfires()), dt);

		//glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		worldRenderer.render(camera, dt, false, NORMALMAPPING, player, usePlayerCamera, killer);


		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		

		

		// Compute frame time
		Time::update(glfwGetTime());

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


void showEndScreen() {
	LOG_TO_CONSOLE("GAME OVER, Sanity is 0 or the killer caught you!", "");
}

void printControls()
{
	std::cout
		<< "  ______                            _   _            _   _ _       _     _   " << "\n"
		<< " |  ____|                          | | | |          | \ | (_)     | |   | |  " << "\n"
		<< " | |__   ___  ___ __ _ _ __   ___  | |_| |__   ___  |  \| |_  __ _| |__ | |_ " << "\n"
		<< " |  __| / __|/ __/ _` | '_ \ / _ \ | __| '_ \ / _ \ | . ` | |/ _` | '_ \| __|" << "\n"
		<< " | |____\__ \ (_| (_| | |_) |  __/ | |_| | | |  __/ | |\  | | (_| | | | | |_ " << "\n"
		<< " |______|___/\___\__,_| .__/ \___|  \__|_| |_|\___| |_| \_|_|\__, |_| |_|\__|" << "\n"
		<< "                      | |                                     __/ |          " << "\n"
		<< "                      |_|                                    |___/           " << "\n"
		<< "=============================================================================" << "\n"

		<< "___Special Controls___" << "\n"
		<< "\t F -- Print current Sanity Level" << "\n"
		<< "\t LMB -- Toggle Light of Player" << "\n"
		<< "___Debug Controls___"
		<< "\t F1 -- Wireframe" << "\n"
		<< "\t F2 -- Backface Culling" << "\n"
		<< "\t F3 -- Toggle Normalmapping (see Water)" << "\n"
		<< "\t F4 -- Reset the Killers Position and the sanity of the Player." << "\n"
		<< "\t F5 -- Switch the Camera to an external debugging Camera (will be useful with physx player controller)"

		<< std::endl;
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
		player->toggleTorch();
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {

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
		player->resetSanity();
		killer->resetKiller();

		break;
	case GLFW_KEY_F5:
		usePlayerCamera = !usePlayerCamera;
		if (!usePlayerCamera) {
			//if switched to free camera, reset it's position to the player
			camera->setPosition(player->getPosition());
		}
		break;
	case GLFW_KEY_F6:

		break;
	case GLFW_KEY_F:
		LOG_TO_CONSOLE("Current Sanity Level: ", player->getSanity());
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
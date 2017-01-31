#define GLM_FORCE_RADIANS

#include "PreHeader.h"


#include "gl_core_4_3.hpp"
#include <GLFW/glfw3.h>
#include "scene.h"
#include "World.h"
#include <iostream>

#include <string>
using std::string;

Scene *scene;
GLFWwindow *window;

//////////////////////////////////////////////////////////
////  Key press callback /////////////////////////////////
//////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	//	if (scene)
	//		scene->animate(!(scene->animating()));
}

////////////////////////////////////////////////////////
//////  Create the scene and initialise ////////////////
////////////////////////////////////////////////////////
void initializeGL() {
	gl::ClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	scene = new World();

	scene->initScene();
}

// Get mouse position every frame
static void cursorPositionCallback(GLFWwindow *Window, double xPos, double yPos)
{
	scene->GetMousePos(window, sf::Vector2i(xPos, yPos));
}

void glfwSetWindowPositionCenter(GLFWwindow* window) {
	// Get windows height and width
	int WindowX, WindowY;
	glfwGetWindowPos(window, &WindowX, &WindowY);

	int Width, Height;
	glfwGetWindowSize(window, &Width, &Height);

	// Get the distance needed to centre the window
	Width *= 0.5;
	Height *= 0.5;

	WindowX += Width;
	WindowY += Height;

	// Accounts for multiple monitors
	int CombinedMonitorsLength;
	GLFWmonitor **MonitorList = glfwGetMonitors(&CombinedMonitorsLength);

	if (MonitorList == NULL) {
		// No monitors detected
		return;
	}

	// Figure out which monitor the window is in
	GLFWmonitor *WindowOwner = NULL;
	int WindowOwnerX, WindowOwnerY, WindowOwnerWidth, WindowOwnerHeight;

	for (int i = 0; i < CombinedMonitorsLength; i++) {
		// Get the monitor position
		int MonitorX, MonitorY;
		glfwGetMonitorPos(MonitorList[i], &MonitorX, &MonitorY);

		// Create video mode to get monitors size
		int MonitorWidth, MonitorHeight;
		GLFWvidmode *monitor_vidmode = (GLFWvidmode*)glfwGetVideoMode(MonitorList[i]);

		if (monitor_vidmode == NULL) {
			// Video mode is required for width and height, so skip this monitor
			continue;

		}
		else {
			MonitorWidth = monitor_vidmode->width;
			MonitorHeight = monitor_vidmode->height;
		}

		// Set the WindowOwner to this monitor if the center of the window is within its bounding box
		if ((WindowX > MonitorX && WindowX < (MonitorX + MonitorWidth)) && (WindowY > MonitorY && WindowY < (MonitorY + MonitorHeight))) {
			WindowOwner = MonitorList[i];

			WindowOwnerX = MonitorX;
			WindowOwnerY = MonitorY;

			WindowOwnerWidth = MonitorWidth;
			WindowOwnerHeight = MonitorHeight;
		}
	}

	if (WindowOwner != NULL) {
		// Set the window position to the center of the monitor which launched the exe
		glfwSetWindowPos(window, WindowOwnerX + (WindowOwnerWidth * 0.5) - Width, WindowOwnerY + (WindowOwnerHeight * 0.5) - Height);
	}
}

////////////////////////////////////////////////////////
/////// Main loop  /////////////////////////////////////
////////////////////////////////////////////////////////
void mainLoop() {
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		//GLUtils::checkForOpenGLError(__FILE__,__LINE__);
		scene->update((float)glfwGetTime());
		scene->render();
		glfwSwapBuffers(window);
		glfwPollEvents();
		// Allows camera view manipulation using mouse
		glfwSetCursorPos(window, 960, 540);
		glfwSetCursorPosCallback(window, cursorPositionCallback);
	}
}

///////////////////////////////////////////////////////
//// resize ///////////////////////////////////////////
///////////////////////////////////////////////////////
void resizeGL(int w, int h) {
	scene->resize(w, h);
}



///////////////////////////////////////////////////////
///////  Main  ////////////////////////////////////////
///////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

	// Initialize GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);


	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);



	// Open the window
	string title = "Game Engine";
	window = glfwCreateWindow(1920, 1080, title.c_str(), NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowPositionCenter(window);
	glfwSetKeyCallback(window, key_callback);

	// Hide mouse position 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);



	// Load the OpenGL functions.
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();

	if (!didLoad) {
		//Claen up and abort
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialization
	initializeGL();

	// Enter the main loop
	mainLoop();

	// Close window and terminate GLFW
	glfwTerminate();

	// Exit program
	exit(EXIT_SUCCESS);
}

#define GLM_FORCE_RADIANS

#include "PreHeader.h"

#include "Scene.h"
#include "World.h"
#include "Editor.h"

using std::string;

Scene *g_pScene;
GLFWwindow *g_pWindow;

bool g_bWindowFocused; // Stores whether the window is in focus

//////////////////////////////////////////////////////////
////  Key press callback /////////////////////////////////
//////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* pWindow, int iKey, int iScancode, int iAction, int iMods)
{
	//if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	//	if (scene)
	//		scene->animate(!(scene->animating()));
}

//////////////////////////////////////////////////////////
////  Window focus callback //////////////////////////////
//////////////////////////////////////////////////////////
static void focus_callback(GLFWwindow *pWindow, int iFocused)
{
	// If the callback is true
	if (iFocused) g_bWindowFocused = true; // Sets global boolean 'focused' to true
	// Else the callback is false
	else g_bWindowFocused = false; // Sets global boolean 'focused' to false
}

//////////////////////////////////////////////////////////
////  Mouse movement callback ////////////////////////////
//////////////////////////////////////////////////////////
static void cursor_callback(GLFWwindow *pWindow, double dX, double dY)
{
	// If window is focused
	if (g_bWindowFocused)
	{
		g_pScene->setMousePos(sf::Vector2i(dX, dY));
	}
}

//////////////////////////////////////////////////////////
//// Window resize callback //////////////////////////////
//////////////////////////////////////////////////////////
static void resize_callback(GLFWwindow *pWindow, int iWidth, int iHeight)
{
	// Resizes the scene to match the window
	g_pScene->resize(iWidth, iHeight);
}

//////////////////////////////////////////////////////////
////  Create the scene and initialise ////////////////////
//////////////////////////////////////////////////////////
void initializeGL() 
{
	// Sets window clear colour
	gl::ClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Gets window width and height
	sf::Vector2i windowSize;
	glfwGetWindowSize(g_pWindow, &windowSize.x, &windowSize.y);

	// Creates a World scene
	//g_pScene = new World(windowSize);
	// TEMPORARY - Creates an Editor scene
	g_pScene = new Editor(windowSize);

	// Initialises scene
	g_pScene->initScene(g_pWindow);
}

//////////////////////////////////////////////////////////
////  Sets the window to center screen ///////////////////
//////////////////////////////////////////////////////////
void glfwSetWindowPositionCenter(GLFWwindow* pWindow)
{
	// Gets window position
	sf::Vector2i windowPosition;
	glfwGetWindowPos(pWindow, &windowPosition.x, &windowPosition.y);

	// Gets window width and height
	sf::Vector2i windowSize;
	glfwGetWindowSize(g_pWindow, &windowSize.x, &windowSize.y);

	// Get the distance needed to centre the window
	windowSize.x *= 0.5;
	windowSize.y *= 0.5;

	windowPosition.x += windowSize.x;
	windowPosition.x += windowSize.y;

	// Accounting for multiple monitors
	int iNumOfMonitors; // Number of monitors
	GLFWmonitor **apMonitorList = glfwGetMonitors(&iNumOfMonitors); // Sets num of monitors

	// If no monitors detected
	if (apMonitorList == NULL) return;

	// Figure out which monitor the window is in
	GLFWmonitor *pWindowOwner = NULL;
	sf::Vector2i windowOwnerSize;
	sf::Vector2i windowOwnerPos;

	for (int i = 0; i < iNumOfMonitors; i++) {
		// Gets the monitor position
		sf::Vector2i monitorPos;
		glfwGetMonitorPos(apMonitorList[i], &monitorPos.x, &monitorPos.y);

		// Create video mode to get monitors size
		sf::Vector2i monitorSize;
		GLFWvidmode *pMonitorVidmode = (GLFWvidmode*)glfwGetVideoMode(apMonitorList[i]);

		if (pMonitorVidmode == NULL)
		{
			// Video mode is required for width and height, so skip this monitor
			continue;
		}
		else 
		{
			monitorSize.x = pMonitorVidmode->width;
			monitorSize.y = pMonitorVidmode->height;
		}

		// Set the WindowOwner to this monitor if the center of the window is within its bounding box
		if ((windowSize.x > monitorPos.x && windowSize.x < (monitorPos.x + monitorSize.x)) &&
			(windowSize.y > monitorPos.y && windowSize.y < (monitorPos.y + monitorSize.y)))
		{
			pWindowOwner = apMonitorList[i];

			windowOwnerPos.x = monitorPos.x;
			windowOwnerPos.y = monitorPos.y;

			windowOwnerSize.x = monitorSize.x;
			windowOwnerSize.y = monitorSize.y;
		}
	}

	// If window owner exists
	if (pWindowOwner != NULL) 
	{
		// Set the window position to the center of the monitor which launched the exe
		glfwSetWindowPos(pWindow, windowOwnerPos.x + (windowOwnerSize.x * 0.5) - windowSize.x, windowOwnerPos.y + (windowOwnerSize.y * 0.5) - windowSize.y);
	}
}

//////////////////////////////////////////////////////////
//// Main loop  //////////////////////////////////////////
//////////////////////////////////////////////////////////
void mainLoop() 
{
	// While the window should remain open and escape is not pressed
	while (!glfwWindowShouldClose(g_pWindow) && !glfwGetKey(g_pWindow, GLFW_KEY_ESCAPE))
	{
		//GLUtils::checkForOpenGLError(__FILE__,__LINE__);

		// If window is focused
		if (g_bWindowFocused)
		{
			// Updates and renders the scene
			g_pScene->update((float)glfwGetTime());
			glfwSetTime(0);
			g_pScene->render();
		}

		glfwSwapBuffers(g_pWindow);
		glfwPollEvents();
	}
}

//////////////////////////////////////////////////////////
////  Main  //////////////////////////////////////////////
//////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	// Initialises GLFW: If it fails the program exits
	if (!glfwInit()) exit(EXIT_FAILURE);

	// Select OpenGL 4.3 with a forward compatible core profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, TRUE);

	// Creates a new glfw window
	g_pWindow = glfwCreateWindow(1920, 1080, string("Game Engine").c_str(), NULL, NULL);
	// If the window isn't created
	if (!g_pWindow)
	{
		// Clean up and abort
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	// Focusses on the window
	glfwMakeContextCurrent(g_pWindow);
	g_bWindowFocused = true;

	// Sets the window to the center of the screen
	glfwSetWindowPositionCenter(g_pWindow);

	// Defines callback functions
	glfwSetKeyCallback(g_pWindow, key_callback);
	glfwSetWindowFocusCallback(g_pWindow, focus_callback);
	glfwSetCursorPosCallback(g_pWindow, cursor_callback);
	glfwSetWindowSizeCallback(g_pWindow, resize_callback);

	// Load the OpenGL functions
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	// If the functions failed to load
	if (!didLoad) 
	{
		// Clean up and abort
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Initialisation
	initializeGL();

	// Enters the main loop
	mainLoop();

	// Deletes Scene ptr
	delete g_pScene;
	// Close window and terminate GLFW
	glfwTerminate();
	// Exit program
	exit(EXIT_SUCCESS);
}

#define GLM_FORCE_RADIANS

#include "PreHeader.h"

#include "Freetype.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Scene.h"
#include "Menu.h"
#include "World.h"
#include "Editor.h"

std::shared_ptr<Scene> g_pScene;
GLFWwindow *g_pWindow;
Freetype g_HUD;

float g_fCurrentTime = 0.0f; // Current time
float g_fPrevTick = 0.0f; // Time of previous update

bool g_bWindowFocused; // Stores whether the window is in focus

//////////////////////////////////////////////////////////
////  Key press callback /////////////////////////////////
//////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* pWindow, int iKey, int iScancode, int iAction, int iMods)
{
	g_pScene->input_key(iKey, iAction);
}

//////////////////////////////////////////////////////////
////  Char input callback ////////////////////////////////
//////////////////////////////////////////////////////////
static void char_callback(GLFWwindow* pWindow, unsigned int uiCodepoint)
{
	g_pScene->input_char(uiCodepoint);
}

//////////////////////////////////////////////////////////
////  Mouse button callback //////////////////////////////
//////////////////////////////////////////////////////////
static void button_callback(GLFWwindow* pWindow, int iButton, int iAction, int iMods)
{
	g_pScene->input_button(iButton, iAction);
}

//////////////////////////////////////////////////////////
////  Mouse scroll callback //////////////////////////////
//////////////////////////////////////////////////////////
static void scroll_callback(GLFWwindow *pWindow, double dX, double dY)
{
	g_pScene->input_scroll(dY);
}

//////////////////////////////////////////////////////////
////  Mouse movement callback ////////////////////////////
//////////////////////////////////////////////////////////
static void cursor_callback(GLFWwindow *pWindow, double dX, double dY)
{
	// If window is focused
	if (g_bWindowFocused)
	{
		g_pScene->setMousePos(sf::Vector2f((float)dX, (float)dY));
	}
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
////  Window resize callback /////////////////////////////
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

	sf::Vector2i windowSize;
	glfwGetWindowSize(g_pWindow, &windowSize.x, &windowSize.y);

	// Creates an initial scene - a Menu
	g_pScene = std::shared_ptr<Scene>(new Menu(g_pWindow, windowSize));

	// Set-up freetype
	gl::Enable(gl::BLEND);
	gl::BlendFunc(gl::SRC_ALPHA, gl::ONE_MINUS_SRC_ALPHA);

	g_HUD.loadCharacters();
	g_HUD.setupBuffers();
	
	// Initialises scene
	g_pScene->initScene(&g_HUD);
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
	windowSize.x = (int)(windowSize.x*0.5f);
	windowSize.y = (int)(windowSize.y*0.5f);

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
		glfwSetWindowPos(pWindow, (int)((float)windowOwnerPos.x + ((float)windowOwnerSize.x * 0.5f) - (float)windowSize.x), (int)((float)windowOwnerPos.y + ((float)windowOwnerSize.y * 0.5f) - (float)windowSize.y));
	}
}

//////////////////////////////////////////////////////////
////  Main loop  /////////////////////////////////////////
//////////////////////////////////////////////////////////
void mainLoop()
{
	// While the window should remain open and escape is not pressed
	while (!glfwWindowShouldClose(g_pWindow))
	{
		// If window is focused
		if (g_bWindowFocused)
		{
			// Defines currentTime and previousTick
			g_fPrevTick = g_fCurrentTime;
			g_fCurrentTime = (float)glfwGetTime();
			// Updates and renders the scene
			g_pScene->update(g_fCurrentTime - g_fPrevTick);
			g_pScene->render();
		}

		glfwSwapBuffers(g_pWindow);
		glfwPollEvents();

		// If Scene has an intent
		if (g_pScene->m_intention != NONE)
		{
			// Gets window size
			sf::Vector2i windowSize;
			glfwGetWindowSize(g_pWindow, &windowSize.x, &windowSize.y);

			// If signalled to enter Menu
			if (g_pScene->m_intention == TO_MENU)
			{
				g_pScene = std::shared_ptr<Scene>(new Menu(g_pWindow, windowSize));
				g_pScene->initScene(&g_HUD);
			}
			// Else If signalled to enter World
			else if (g_pScene->m_intention == TO_GAME)
			{
				g_pScene = std::shared_ptr<Scene>(new World(g_pWindow, windowSize));
				g_pScene->initScene(&g_HUD);
			}
			// Else If signalled to enter Editor
			else if (g_pScene->m_intention == TO_EDITOR)
			{
				g_pScene = std::shared_ptr<Scene>(new Editor(g_pWindow, windowSize));
				g_pScene->initScene(&g_HUD);
			}
		}
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
	g_pWindow = glfwCreateWindow(1920, 1080, string("TREBuchet - The greatest medieval engine.").c_str(), NULL, NULL);
	
	// If the window isn't created
	if (!g_pWindow) { closeProgram(); }
	
	// Focusses on the window
	glfwMakeContextCurrent(g_pWindow);
	g_bWindowFocused = true;

	// Sets the window to the center of the screen
	glfwSetWindowPositionCenter(g_pWindow);

	// Defines callback functions
	glfwSetKeyCallback(g_pWindow, key_callback);
	glfwSetCharCallback(g_pWindow, char_callback);
	glfwSetMouseButtonCallback(g_pWindow, button_callback);
	glfwSetCursorPosCallback(g_pWindow, cursor_callback);
	glfwSetScrollCallback(g_pWindow, scroll_callback);
	glfwSetWindowFocusCallback(g_pWindow, focus_callback);
	glfwSetWindowSizeCallback(g_pWindow, resize_callback);

	// Load the OpenGL functions
	gl::exts::LoadTest didLoad = gl::sys::LoadFunctions();
	// If the functions failed to load
	if (!didLoad) { closeProgram(); }

	// Initialisation
	initializeGL();
	
	// Resets elapsed time before mainloop begins
	glfwSetTime(0);

	// Enters the main loop
	mainLoop();

	closeProgram();
}
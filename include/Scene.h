#ifndef SCENE_H
#define SCENE_H

#include "PreHeader.h"
#include "Camera.h"
#include <Freetype.h>

// Abstract class in order to set up world
class Scene
{
	public:

		Scene() {}
		Scene(GLFWwindow *pWindow) {};
	
		sf::Vector2i m_windowSize; // Dimensions of window
	
		sf::Sound Music;

		// Load in all texture and initilise shaders
		virtual void initScene(Freetype* pOverlay) = 0;
	
		virtual void setMousePos(sf::Vector2f mousepos) = 0;
	
		// Called on keyPress event
		virtual void keyPress(const int kiKey) = 0;
		// Called on mouseScroll event
		virtual void mouseScroll(const double kdDelta) = 0;

		// Run every frame
		virtual void update(float fTimeElapsed) = 0;
	
		// Draw Scene
		virtual void render() = 0;
	
		// Aloow screen to be resized without causing rendering issues
		void resize(int iWidth, int iHeight)
		{
			m_windowSize.x = iWidth;
			m_windowSize.y = iHeight;
	
			gl::Viewport(0, 0, m_windowSize.x, m_windowSize.y);
			m_camera.setAspectRatio((float)m_windowSize.x / m_windowSize.y);
		}
	
		sf::Vector2i getWindowSize() { return m_windowSize; }
	    
	protected:

		GLSLProgram m_worldShader;
		GLSLProgram m_freeType;
		GLSLProgram m_imageType;

		Camera m_camera; // Camera which user can control

		Freetype* m_pHUD;

		GLFWwindow *m_pWindow; // The window
		sf::Vector2f m_mousePos; // Holds mouse cursor position
};

#endif // SCENE_H

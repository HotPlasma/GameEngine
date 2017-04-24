#ifndef SCENE_H
#define SCENE_H

#include "PreHeader.h"
#include <Freetype.h>

// Abstract class in order to set up world
class Scene
{
	public:
		Scene() : m_bAnimate(true) {}
	
		sf::Vector2i m_windowSize; // Dimensions of window
	
		sf::Sound Music;

		// Load in all texture and initilise shaders
		virtual void initScene(Freetype* pOverlay) = 0;
	
		virtual void setMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos) = 0;
	
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
		}
	
		void animate(bool bValue) { m_bAnimate = bValue; }
		bool animating() { return m_bAnimate; }
	
		sf::Vector2i getWindowSize() { return m_windowSize; }
	    
	protected:
		bool m_bAnimate;
};

#endif // SCENE_H

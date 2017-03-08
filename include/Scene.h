#ifndef SCENE_H
#define SCENE_H

#include "PreHeader.h"

// Abstract class in order to set up world
class Scene
{
	public:
		Scene() : m_bAnimate(true) {}
	
		sf::Vector2i m_windowSize; // Dimensions of window
	
		// Load in all texture and initilise shaders
		virtual void initScene(GLFWwindow *pWindow) = 0;
	
		virtual void setMousePos(sf::Vector2i mousepos) = 0;
	
		// Run every frame
		virtual void update(float t) = 0;
	
		// Draw Scene
		virtual void render() = 0;
	
		// Aloow screen to be resized without causing rendering issues
		void resize(int w, int h)
		{
			m_windowSize.x = w;
			m_windowSize.y = h;
	
			gl::Viewport(0, 0, m_windowSize.x, m_windowSize.y);
		}
	
		void animate(bool bValue) { m_bAnimate = bValue; }
		bool animating() { return m_bAnimate; }
	
		sf::Vector2i getWindowSize() { return m_windowSize; }
	    
	protected:
		bool m_bAnimate;
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H
#include <GLFW/glfw3.h>

// Abstract class in order to set up world
class Scene
{
	public:
		Scene() : m_animate(true) {}
	
		sf::Vector2i m_windowSize; // Dimensions of window
	
		// Load in all texture and initilise shaders
		virtual void initScene() = 0;
	
		virtual void setMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos) = 0;
	
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
	
		void animate(bool bValue) { m_animate = bValue; }
		bool animating() { return m_animate; }
	
		sf::Vector2i getWindowSize() { return m_windowSize; }
	    
	protected:
		bool m_animate;
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H
#include <GLFW/glfw3.h>

// Abstract class in order to set up world
class Scene
{
public:
	Scene() : m_animate(true) { }
	
  
    // Load in all texture and initilise shaders
    virtual void initScene() = 0;

	virtual void GetMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos) = 0;


	// Run every frame
    virtual void update( float t ) = 0;

    // Draw Scene
    virtual void render() = 0;


	// Aloow screen to be resized without causing rendering issues
    virtual void resize(int, int) = 0;
    
    void animate( bool value ) { m_animate = value; }
    bool animating() { return m_animate; }
    
protected:
	bool m_animate;
};

#endif // SCENE_H

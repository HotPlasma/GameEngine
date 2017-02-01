#ifndef WORLD_H
#define WORLD_H



#include "Texture.h"

#include "scene.h"
#include "Camera.h"
#include "SceneReader.h"

class World : public Scene
{
	private:
		glm::vec3 m_collectableSpeed; // Speed at which a collectable bounces
	
		unsigned int m_uiBatteriesCollected = 0; // Batteries that have been collected
	
		GLuint m_programHandle; // Program context
		glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale
	
		void linkMe(GLint vertShader, GLint fragShader); // Connects shaders
	
		Texture *g_pTexture; // Holds texture
		Camera m_camera; // Camera which user can control
		GLFWwindow *m_pWindow; // The window
		sf::Vector2i m_mousePos; // Holds mouse cursor position
		SceneReader m_sceneReader; // Reads .xml file in order to create world
	
	public:
		World(sf::Vector2i windowSize);
		void initScene();
		void setMousePos(GLFWwindow *pWindow, sf::Vector2i mousepos);
		void update(float t);
		void modelUpdate(int index);
		void render();
};
#endif  

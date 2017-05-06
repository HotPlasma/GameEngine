#ifndef WORLD_H
#define WORLD_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"
#include "glslprogram.h"

class World : public Scene
{
	private:
		GLSLProgram m_WorldShader;
		glm::vec3 m_collectableSpeed; // Speed at which a collectable bounces
		glm::vec3 m_aiSpeed; //ai movement speed
		glm::vec3 m_aiRotation; //ai rotation
		glm::vec3 m_collidableRotation;
		bool aiSearching = true; 
		//clock_t timer;
		double rotationAngle;
		float movementSpeed = 0.0005;
		mat4 movement;
		sf::Clock batteryTimer;
		sf::Clock levelTimer;
		sf::Clock aiWander;
		float spareBatteries = 0;
		float batteryLife = 100;
		
		
	

	

	
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

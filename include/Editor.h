#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"

class Editor : public Scene
{
	private:
	
		glm::vec3 m_collectableSpeed; // Speed at which a collectable bounces
	
		GLuint m_programHandle; // Program context
		glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale
	
		void linkMe(GLint vertShader, GLint fragShader); // Connects shaders
	
		Texture *g_pTexture; // Holds texture
		Camera m_camera; // Camera which user can control
		GLFWwindow *m_pWindow; // The window
		sf::Vector2i m_mousePos; // Holds mouse cursor position
	
	public:
	
		Editor(sf::Vector2i windowSize);
	
		void initScene(GLFWwindow *pWindow);
		void setMousePos(sf::Vector2i mousepos);
		void update(float t);
		void modelUpdate(int index);
		void render();
};
#endif  

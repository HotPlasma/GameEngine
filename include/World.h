#ifndef WORLD_H
#define WORLD_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"
#include "glslprogram.h"
#include "Freetype.h"

class World : public Scene
{
	private:
		GLSLProgram m_worldShader;
		GLSLProgram m_freeType;
		GLSLProgram m_imageType;
		glm::vec3 m_collectableSpeed; // Speed at which a collectable bounces
		Freetype* m_pHUD;
	
		GLuint m_programHandle; // Program context
		glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale
	
		void linkShaders(); // Connects shaders

		void setMatrices(GLSLProgram * shader, mat4 model, mat4 view, mat4 projection);
	
		GLFWwindow *m_pWindow; // The window

		sf::Vector2f m_mousePos; // Holds mouse cursor position

		SceneReader m_sceneReader; // Reads .xml file in order to create world
		glm::mat4 m_V, m_P;
	
	public:
		World(GLFWwindow *pWindow, sf::Vector2i windowSize);
		void initScene(Freetype* pOverlay);
		void setMousePos(sf::Vector2f mousepos);
		void update(float fTimeElapsed);
		void render();
};
#endif  

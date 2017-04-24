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
		glm::vec3 m_collectableSpeed; // Speed at which a collectable bounces
		Freetype* m_pHUD;
	
		GLuint m_programHandle; // Program context
		glm::mat4 m_M; // Allows manipulation of each models position, rotation and scale
	
		void linkShaders(); // Connects shaders

		void setLightParamaters(GLSLProgram * pShader);
		void SetMatices(GLSLProgram * pShader, mat4 model, mat4 view, mat4 projection);
	
		Texture *g_pTexture; // Holds texture
		Camera m_camera; // Camera which user can control
		GLFWwindow *m_pWindow; // The window
		sf::Vector2i m_mousePos; // Holds mouse cursor position
		SceneReader m_sceneReader; // Reads .xml file in order to create world
		glm::mat4 m_V, m_P;
	
	public:
		World(sf::Vector2i windowSize);
		void initScene(Freetype* pOverlay);
		void setMousePos(GLFWwindow *pWindow, sf::Vector2i mousepos);
		void update(float fTimeElapsed);
		void render();
};
#endif  

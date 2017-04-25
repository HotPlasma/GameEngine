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

		SceneReader m_sceneReader; // Reads .xml file in order to create world

		void linkShaders(); // Connects shaders
		void setMatrices(GLSLProgram * pShader, const mat4 kModel, const mat4 kView, const mat4 kProjection);
	
	public:

		World(GLFWwindow *pWindow, const sf::Vector2i kWindowSize);

		void initScene(Freetype* pOverlay);
		void update(const float kfTimeElapsed);
		void render();

		void setMousePos(const sf::Vector2f kMousePos);
};
#endif  

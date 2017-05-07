#ifndef WORLD_H
#define WORLD_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"
#include "glslprogram.h"
#include "Freetype.h"
#include "Utilities.h"


struct CollisionInfo
{
	bool isIntersecting = false;
	glm::vec3 minTranslateVec = glm::vec3(0,0,0);
	float penetration = 0;
};

class World : public Scene
{
	private:

		SceneReader m_sceneReader; // Reads .xml file in order to create world

		bool m_collectGoingUp = true; // Whether collectables are moving up
		float m_collectYOffset = 0.0f; // Y axis offset for collectables
		Bounds m_collectBounds = Bounds(3.0f, 1.0f); // Upper and lower bounds for Y offset

		void setMatrices(GLSLProgram * pShader, const mat4 kModel, const mat4 kView, const mat4 kProjection);

		bool m_bCollisionInit = false;

		Model m_Player;

	public:

		World(GLFWwindow *pWindow, const sf::Vector2i kWindowSize);

		void initScene(Freetype* pOverlay);

		void keyPress(const int kiKey) {}
		void mouseScroll(const double kdDelta) {}

		void update(const float kfTimeElapsed);
		void render();
};
#endif  

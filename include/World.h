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
#include "reactphysics3d.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"

class World : public Scene
{
	private:

		SceneReader m_sceneReader; // Reads .xml file in order to create world

		bool m_collectGoingUp = true; // Whether collectables are moving up
		float m_collectYOffset = 0.0f; // Y axis offset for collectables
		Bounds m_collectBounds = Bounds(3.0f, 1.0f); // Upper and lower bounds for Y offset

		void setMatrices(GLSLProgram * pShader, const mat4 kModel, const mat4 kView, const mat4 kProjection);

	/*	rp3d::CollisionWorld m_CollisonWorld;

		rp3d::CollisionBody * CameraBody;*/

		Model m_Player;

		//vector<rp3d::CollisionBody*> ModelBodies;
		//rp3d::CollisionCallback * CameraCallback;

		btBroadphaseInterface* broadphase;

		btDynamicsWorld * m_dynamicWorld;
		btDispatcher * m_dispatcher;
		btCollisionConfiguration * m_collisionCofig; 
		btBroadphaseInterface * m_broadphase;
		btConstraintSolver * m_constraintSolver;

		btRigidBody *CameraRigidBody;
		btRigidBody *rigidBody;

		vector<btRigidBody*> m_CollisionBodies;
	
	public:

		World(GLFWwindow *pWindow, const sf::Vector2i kWindowSize);

		void initScene(Freetype* pOverlay);

		void keyPress(const int kiKey) {}
		void mouseScroll(const double kdDelta) {}

		void update(const float kfTimeElapsed);
		void render();
};
#endif  

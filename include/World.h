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
//#include "btBulletDynamicsCommon.h"
//#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"
//#include "LinearMath\btIDebugDraw.h"

//struct bulletObject
//{
//	int id;
//	bool hit;
//	btRigidBody * body;
//	
//};

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

		/*rp3d::BoxShape* TheBoxShape;

		rp3d::BoxShape* TheBoxShape2;*/

	/*	rp3d::CollisionWorld m_CollisonWorld;

		rp3d::CollisionBody * CameraBody;*/

		Model m_Player;

		CollisionInfo CInfo;

		//rp3d::CollisionWorld m_collisionWorld;

		//vector<rp3d::CollisionBody*> m_collisionBodies;

		//rp3d::CollisionBody * PlayerBody;

		//rp3d::CollisionBody * StumpBody;

		//rp3d::CollisionCallback * CollisionCB;

		//rp3d::ContactPointInfo * collisionInfo;

		bool TestCollision(Model CollisionShape1, Model CollisionShape2, CollisionInfo &CollisionResult);

		bool TestCollisionAxis(glm::vec3 axis, float minA, float maxA, float minB, float maxB, glm::vec3 &mtvAxis, float &mtvDistance);

		//vector<rp3d::CollisionBody*> ModelBodies;
		//rp3d::CollisionCallback * CameraCallback;

		//btBroadphaseInterface* broadphase;

		//btDynamicsWorld * m_collisionWorld;
		//btDispatcher * m_dispatcher;
		//btCollisionConfiguration * m_collisionCofig; 
		//btBroadphaseInterface * m_broadphase;
		//btConstraintSolver * m_constraintSolver;

		//btRigidBody *CameraRigidBody;
		//btRigidBody *rigidBody;

		//vector<btRigidBody*> m_CollisionBodies;
		
		//btIDebugDraw DebugDrawer;

		//void initPhysics();



	//	btRigidBody* addSphere(float rad, float x, float y, float z, float mass);

		/*void renderSphere(btRigidBody * sphere);
		void renderPlane(btRigidBody * Plane);*/

		//Model tester;
	public:

		World(GLFWwindow *pWindow, const sf::Vector2i kWindowSize);

		void initScene(Freetype* pOverlay);

		void keyPress(const int kiKey) {}
		void mouseScroll(const double kdDelta) {}

		void update(const float kfTimeElapsed);
		void render();
};
#endif  

#include "World.h"

using std::string;
using std::ifstream;

#define COLLECTABLE_ROTATION 90.0f
#define COLLECTABLE_SPEED 1.5f

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 15.0f

//btRigidBody * World::addSphere(float rad, float x, float y, float z, float mass)
//{
//	btTransform t;
//	t.setIdentity();
//	t.setOrigin(btVector3(x, y, z));
//
//	btSphereShape * Sphere = new btSphereShape(rad);
//	btVector3 inertia(0, 0, 0);
//	if (mass != 0)
//	{
//		Sphere->calculateLocalInertia(mass, inertia);
//	}
//	btMotionState * motion = new btDefaultMotionState(t);
//
//	btRigidBody::btRigidBodyConstructionInfo info(mass, motion, Sphere);
//
//	btRigidBody * body = new btRigidBody(info);
//
//	m_collisionWorld->addRigidBody(body);
//	m_CollisionBodies.push_back(body);
//		
//	//tester = Model("assets/models/Rock.obj", "ssets/textures/Rock.bmp", glm::vec3(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 1);
//
//	return body;
//}

//void World::renderSphere(btRigidBody * sphere)
//{
//	float r = ((btSphereShape*)sphere->getCollisionShape())->getRadius();
//	btTransform t;
//	sphere->getMotionState()->getWorldTransform(t);
//	glm::mat4 mat;
//	t.getOpenGLMatrix(glm::value_ptr(mat));
//	tester.m_M = tester.m_M * mat;
//}
//
//void World::renderPlane(btRigidBody * Plane)
//{
//	btTransform t;
//	Plane->getMotionState()->getWorldTransform(t);
//	glm::mat4 mat;
//	t.getOpenGLMatrix(glm::value_ptr(mat));
//	m_sceneReader.m_modelList[1].m_M = m_sceneReader.m_modelList[1].m_M * mat;
//}

//bool contact_callback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
//{
//	cout << ((btRigidBody*)index0)->getUserPointer() << " " << ((btRigidBody*)index1)->getUserPointer() << std::endl;
//		
//	cout << "Collision" << endl;
//
//	return false;
//}

World::World(GLFWwindow *pWindow, sf::Vector2i windowSize)
{

	m_pWindow = pWindow;

	m_windowSize = windowSize;

	m_camera.setAspectRatio((float)windowSize.x / windowSize.y);

	//gContactAddedCallback = contact_callback;
}


void World::initScene(Freetype* pOverlay)
{
	m_pHUD = pOverlay; // Get the Heads up display for the scene

	linkShaders();
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	m_sceneReader = SceneReader("assets/scenes/Scene.xml");

	m_Player = Model("assets/models/Player.obj", "assets/textures/default.bmp", glm::vec3(10, -5, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), 1);

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList[i].initModel();
		}
		/*world.ModelList[i].DrawModel(true, true);*/
	}

	m_Player.loadModel();
	m_Player.initModel();
	//m_Player.setVisable(true);


//	// Initial position and orientation of the collision body
//	rp3d::Vector3 initPosition(0, 0, 0);
//	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
//	rp3d::Transform transform(initPosition, initOrientation);
//	// Create a collision body in the world
//	
//
//	PlayerBody = m_collisionWorld.createCollisionBody(transform);
//
//	rp3d::BoxShape* TheBoxShape = new rp3d::BoxShape(rp3d::Vector3(3.15,6.76,3.15));
//	//rp3d::ConvexMeshShape* TheBoxShape = new rp3d::ConvexMeshShape(m_Player.m_positionData.data(), m_Player.m_positionData.size(), sizeof(float) / 3);
////	TheBoxShape = &shape;
//	
//	PlayerBody->addCollisionShape(TheBoxShape, transform);
//
//	// Initial position and orientation of the collision body
//	rp3d::Vector3 initPosition2(m_sceneReader.m_modelList.at(4).getPosition().x , m_sceneReader.m_modelList.at(4).getPosition().y, m_sceneReader.m_modelList.at(4).getPosition().z);
//	rp3d::Quaternion initOrientation2 = rp3d::Quaternion::identity();
//	rp3d::Transform transform2(initPosition2, initOrientation2);
//	// Create a collision body in the world
//
//
//	StumpBody = m_collisionWorld.createCollisionBody(transform2);
//
//	rp3d::BoxShape* TheBoxShape2 = new rp3d::BoxShape(rp3d::Vector3(2.8,2.8,2.8));
//
//	//rp3d::ConvexMeshShape* TheBoxShape2 = new rp3d::ConvexMeshShape(m_sceneReader.m_modelList.at(3).m_positionData.data(), m_sceneReader.m_modelList.at(3).m_positionData.size(), sizeof(float) / 3);
//
//	//	TheBoxShape = &shape;
//	StumpBody->addCollisionShape(TheBoxShape2, transform2);
	
	//m_collisionBodies.push_back(PlayerBody);





	////Physics
	//m_collisionCofig = new btDefaultCollisionConfiguration();
	//m_dispatcher = new btCollisionDispatcher(m_collisionCofig);
	//m_broadphase = new btDbvtBroadphase();
	//m_constraintSolver = new btSequentialImpulseConstraintSolver();

	//m_collisionWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_constraintSolver, m_collisionCofig);
	//m_collisionWorld->setGravity(btVector3(0, 0, 0));

	//btTransform t;
	//t.setIdentity();
	//t.setOrigin(btVector3(m_sceneReader.m_modelList.at(3).getPosition().x, m_sceneReader.m_modelList.at(3).getPosition().y, m_sceneReader.m_modelList.at(3).getPosition().z));

	//btBoxShape * stump = new btBoxShape(btVector3(20,20,20));

	//btMotionState * motion = new btDefaultMotionState(t);

	//btRigidBody::btRigidBodyConstructionInfo info(0, motion, stump);

	//btRigidBody * StumpBody = new btRigidBody(info);

	//StumpBody->setCollisionFlags(StumpBody->getCollisionFlags() || btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	//m_CollisionBodies.push_back(StumpBody);
	//m_collisionWorld->addRigidBody(m_CollisionBodies.at(0));
	//StumpBody->setUserPointer(m_CollisionBodies.at(0));


	//btTransform tP;
	//tP.setIdentity();
	//tP.setOrigin(btVector3(m_Player.getPosition().x, m_Player.getPosition().y, m_Player.getPosition().z));

	//btBoxShape * PBox = new btBoxShape(btVector3(20, 20, 20));

	//btMotionState * PMotion = new btDefaultMotionState(tP);

	//btRigidBody::btRigidBodyConstructionInfo PlayerInfo(0, PMotion, PBox);

	//btRigidBody * PlayBody = new btRigidBody(PlayerInfo);

	//StumpBody->setCollisionFlags(PlayBody->getCollisionFlags() || btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

	//m_CollisionBodies.push_back(PlayBody);

	//m_collisionWorld->addRigidBody(m_CollisionBodies.at(1));
	//PlayBody->setUserPointer(m_CollisionBodies.at(1));

	

	// Initial position and orientation of the collision body
	/*rp3d::Vector3 initPosition(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
	rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(initPosition, initOrientation);*/
	// Create a collision body in the world

	//CameraBody = m_CollisonWorld.createCollisionBody(transform);

	//rp3d::CylinderShape cylinderShape(1.0, 3.0);

	//CameraBody->addCollisionShape(&cylinderShape, rp3d::Transform::identity());

	//for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	//{

	//	rp3d::ConcaveMeshShape MeshShape = new rp3d::ConcaveMeshShape(&mPhysicsTriangleMesh);
	//	ModelBodies.resize(ModelBodies.size() + 1);
	//	ModelBodies.at(i) = m_CollisonWorld.createCollisionBody(rp3d::Transform(rp3d::Vector3(m_sceneReader.m_modelList.at(i).getPosition().x, m_sceneReader.m_modelList.at(i).getPosition().y, m_sceneReader.m_modelList.at(i).getPosition().z), rp3d::Quaternion::identity()));
	//	ModelBodies.at(i)->addCollisionShape(&MeshShape, rp3d::Transform::identity());
	//}

	//m_CollisonWorld.testCollision(CameraBody, CameraCallback);

	


	//m_CollisonWorld.
//	HUD->LoadHUDImage("assets/textures/Flag_of_Wales.png", 500.f, 500.f, -90, 30.0f);
}

void World::setMatrices(GLSLProgram * pShader, const mat4 kModel, const mat4 kView, const mat4 kProjection)
{
	mat4 mv = kView * kModel;
	pShader->setUniform("ModelViewMatrix", mv);
	pShader->setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	pShader->setUniform("MVP", kProjection * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(kModel)));
	pShader->setUniform("M", kModel);
	pShader->setUniform("V", kView);
	pShader->setUniform("P", kProjection);
}

bool World::TestCollision(Model CollisionShape1, Model CollisionShape2, CollisionInfo &CollisionResult)
{
	//float mtvDistance = 999999999.f;
	//glm::vec3 mtvAxis;

	//if (!TestCollisionAxis(glm::vec3(1, 0, 0), CollisionShape1.getCollisionBox().min.x, CollisionShape1.getCollisionBox().max.x, CollisionShape2.getCollisionBox().min.x, CollisionShape2.getCollisionBox().max.x, mtvAxis, mtvDistance))
	//{
	//	return false;
	//}

	//if (!TestCollisionAxis(glm::vec3(0, 1, 0), CollisionShape1.getCollisionBox().min.y, CollisionShape1.getCollisionBox().max.y, CollisionShape2.getCollisionBox().min.y, CollisionShape2.getCollisionBox().max.y, mtvAxis, mtvDistance))
	//{
	//	return false;
	//}

	//if (!TestCollisionAxis(glm::vec3(0, 0, 1), CollisionShape1.getCollisionBox().min.z, CollisionShape1.getCollisionBox().max.z, CollisionShape2.getCollisionBox().min.z, CollisionShape2.getCollisionBox().max.z, mtvAxis, mtvDistance))
	//{
	//	return false;
	//}
	//
	//CollisionResult.isIntersecting = true;

	//CollisionResult.minTranslateVec = glm::vec3(glm::normalize(mtvAxis));

	//CollisionResult.penetration = (float)glm::sqrt(mtvDistance) * 1.001f;
}

bool World::TestCollisionAxis(glm::vec3 axis, float minA, float maxA, float minB, float maxB, glm::vec3 &mtvAxis, float &mtvDistance)
{
	//float axisLengthSquared = glm::dot(axis, axis);

	//if (axisLengthSquared < 1.0e-8f)
	//{
	//	return true;
	//}

	//float d0 = (maxB - minA);   // 'Left' side
	//float d1 = (maxA - minB);   // 'Right' side

	//if (d0 <= 0.0f || d1 <= 0.0f)
	//{
	//	return false;
	//}

	//float overlap = (d0 < d1) ? d0 : -d1;

	//glm::vec3 sep = axis * (overlap / axisLengthSquared);

	//float sepLengthSquared = glm::dot(sep, sep);

	//if (sepLengthSquared < mtvDistance)
	//{
	//	mtvDistance = sepLengthSquared;
	//	mtvAxis = sep;
	//}

	//return true;

}

void World::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));

	m_camera.rotate(delta.x*CAMERA_ROTATION, delta.y*CAMERA_ROTATION);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
		m_Player.setPosition(m_Player.getPosition() + glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_camera.move(glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
		m_Player.setPosition(m_Player.getPosition() + glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
		m_Player.setPosition(m_Player.getPosition() + glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_camera.move(glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
		m_Player.setPosition(m_Player.getPosition() + glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}



	//// New position and orientation of the collision body
	//rp3d::Vector3 position(m_camera.getPosition().x, m_camera.getPosition().y - 5, m_camera.getPosition().z);
	//rp3d::Quaternion orientation = rp3d::Quaternion::identity();
	//rp3d::Transform newTransform(position, orientation);



	//PlayerBody->setTransform(newTransform);

	//cout << PlayerBody->getTransform().getPosition().x << " " << PlayerBody->getTransform().getPosition().y << " " << PlayerBody->getTransform().getPosition().z << endl;

	//if (m_collisionWorld.testAABBOverlap(PlayerBody, StumpBody))
	//{
	//	cout << "Colliding" << endl;
	//}
	//else
	//{
	//	cout << "Not Colliding" << endl;
	//}

	//system("CLS");
	//m_collisionWorld.testCollision(PlayerBody, StumpBody, CollisionCB);

	//CollisionCB->notifyContact(*collisionInfo);

	//collisionInfo->

//	m_collisionWorld.testCollision()

	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		btRigidBody* sphere = m_CollisionBodies.at(1);
		btVector3 look(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z);
		sphere->setLinearVelocity(look);
		m_sceneReader.m_modelList.at(3).setPosition(glm::vec3(sphere->getLinearVelocity().x, sphere->getLinearVelocity().y, sphere->getLinearVelocity().z));
	}*/

	//btTransform trans, trans2;

	//m_CollisionBodies.at(1)->getMotionState()->getWorldTransform(trans);

	//m_CollisionBodies.at(0)->getMotionState()->getWorldTransform(trans2);

	//trans.setOrigin(btVector3(m_Player.getPosition().x, m_Player.getPosition().y, m_Player.getPosition().z));

	//m_CollisionBodies.at(1)->setWorldTransform(trans);


	//m_collisionWorld->stepSimulation(kfTimeElapsed);

	//std::cout << "Play Pos: " << trans.getOrigin().getX() << " " << trans.getOrigin().getY() << " " << trans.getOrigin().getZ() << std::endl; std::cout << "Stump Pos: " << trans2.getOrigin().getX() << " " << trans2.getOrigin().getY() << " " << trans2.getOrigin().getZ() << std::endl;



	//m_collisionWorld->contactPairTest(m_CollisionBodies.at(1), m_CollisionBodies.at(2), )

	//system("CLS");

	// Sticks the camera to y 0.0
	m_camera.setPosition(glm::vec3(m_Player.getPosition().x, 0.0f, m_Player.getPosition().z));
	//m_Player.setPosition(glm::vec3(m_camera.getPosition().x, -5.f, m_camera.getPosition().z));

	//	CameraBody->setTransform(rp3d::Transform(rp3d::Vector3(m_camera.getPosition().x, 0.0f, m_camera.getPosition().z), rp3d::Quaternion::identity()));

		//rp3d::Vector3 position(m_camera.getPosition().x, 0, m_camera.getPosition().z);
		//rp3d::Quaternion orientation = rp3d::Quaternion::identity();
		//rp3d::Transform newTransform(position, orientation);
		// Move the collision body
		//CameraBody->setTransform(newTransform);


		/////////////////// COLLECTABLE BOBBING ///////////////////
		// If collectables are moving up and offset is greater than upper bound
	if (m_collectGoingUp && m_collectYOffset >= m_collectBounds.upper())
	{
		// Move collectable down
		m_collectGoingUp = false;
	}

	// If collectables are moving down and offset is less than lower bound
	if (!m_collectGoingUp && m_collectYOffset <= m_collectBounds.lower())
	{
		// Move collectable up
		m_collectGoingUp = true;
	}

	// Increments offset up or down
	if (m_collectGoingUp) { m_collectYOffset += COLLECTABLE_SPEED*kfTimeElapsed; }
	else if (!m_collectGoingUp) { m_collectYOffset -= COLLECTABLE_SPEED*kfTimeElapsed; }

	// Clamps offset to bounds
	m_collectYOffset = glm::clamp(m_collectYOffset, m_collectBounds.lower(), m_collectBounds.upper());

	// For all Models
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		// If collectable
		if (m_sceneReader.m_modelList.at(i).isCollectable())
		{
			// If not collected
			if (!m_sceneReader.m_modelList.at(i).getCollected())
			{
				// Rotates collectable
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(0, m_sceneReader.m_modelList.at(i).getRotation().y + (COLLECTABLE_ROTATION*kfTimeElapsed), m_sceneReader.m_modelList.at(i).getRotation().z));

				// Get distance between player and collectable
				glm::vec3 distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				// If collision with a collectable
				if (sqrtf(powf(distance.x, 2.0f) + powf(distance.z, 2.0f)) < 5)
				{
					// Marks it as collected
					m_sceneReader.m_modelList.at(i).setCollected(true);
				}
			}
		}
	}


	for (int j = 3; j < m_sceneReader.m_modelList.size(); j++)
	{
		if (m_Player.getCollisionBox().right > m_sceneReader.m_modelList.at(j).getCollisionBox().left &&
			m_Player.getCollisionBox().left < m_sceneReader.m_modelList.at(j).getCollisionBox().right &&
			m_Player.getCollisionBox().top > m_sceneReader.m_modelList.at(j).getCollisionBox().bottom &&
			m_Player.getCollisionBox().bottom < m_sceneReader.m_modelList.at(j).getCollisionBox().top &&
			m_Player.getCollisionBox().back > m_sceneReader.m_modelList.at(j).getCollisionBox().front &&
			m_Player.getCollisionBox().front < m_sceneReader.m_modelList.at(j).getCollisionBox().back)
		{
			cout << "Collision with " << m_sceneReader.m_modelList.at(j).getName() << endl;

			glm::vec3 Distance(m_sceneReader.m_modelList.at(j).getPosition() - m_Player.getPosition());
			//Distance = glm::normalize(Distance);
			//Diff in X to move out

			/*glm::vec3 Diff(
			abs(std::min(m_Player.getCollisionBox().right - m_sceneReader.m_modelList.at(j).getCollisionBox().left,
			m_sceneReader.m_modelList.at(j).getCollisionBox().right - m_Player.getCollisionBox().left)),
			-5,
			abs(std::min(m_Player.getCollisionBox().back - m_sceneReader.m_modelList.at(j).getCollisionBox().front,
			m_sceneReader.m_modelList.at(j).getCollisionBox().back - m_Player.getCollisionBox().front))
			);*/

			if (m_Player.getCollisionBox().right > m_sceneReader.m_modelList.at(j).getCollisionBox().left) //if player right is inside object left side 
			{
				float tempXDiff = (m_Player.getCollisionBox().right - m_sceneReader.m_modelList.at(j).getCollisionBox().left);
				m_Player.setPosition(glm::vec3(m_Player.getPosition().x - tempXDiff, m_Player.getPosition().y, m_Player.getPosition().z));
			}
			if (m_Player.getCollisionBox().left < m_sceneReader.m_modelList.at(j).getCollisionBox().right) //if player left is inside object right side 
			{
				float tempXDiff = (m_Player.getCollisionBox().left - m_sceneReader.m_modelList.at(j).getCollisionBox().right);
				m_Player.setPosition(glm::vec3(m_Player.getPosition().x + tempXDiff, m_Player.getPosition().y, m_Player.getPosition().z));
			}
			if (m_Player.getCollisionBox().back > m_sceneReader.m_modelList.at(j).getCollisionBox().front) //if player back is inside object front side 
			{
				float tempZDiff = (m_sceneReader.m_modelList.at(j).getCollisionBox().front - m_Player.getCollisionBox().back);
				m_Player.setPosition(glm::vec3(m_Player.getPosition().x, m_Player.getPosition().y, m_Player.getPosition().z + tempZDiff));
			}
			if (m_Player.getCollisionBox().front < m_sceneReader.m_modelList.at(j).getCollisionBox().back) //if player front is inside object front side 
			{
				float tempZDiff = (m_sceneReader.m_modelList.at(j).getCollisionBox().front - m_Player.getCollisionBox().back);
				m_Player.setPosition(glm::vec3(m_Player.getPosition().x, m_Player.getPosition().y, m_Player.getPosition().z + tempZDiff));
			}
			//calculate distance required to move out of object
			/*float tempXDiff = Distance.x - (m_Player.getCollisionBox().right - m_sceneReader.m_modelList.at(j).getCollisionBox().left);
			float tempZDiff = Distance.y - (m_Player.getCollisionBox().back - m_sceneReader.m_modelList.at(j).getCollisionBox().front);
			m_Player.setPosition(glm::vec3(m_Player.getPosition().x + tempXDiff, m_Player.getPosition().y, m_Player.getPosition().z + tempZDiff));*/

			

		/*	abs(std::min(m_Player.getCollisionBox().bottom - m_sceneReader.m_modelList.at(j).getCollisionBox().top,
				m_sceneReader.m_modelList.at(j).getCollisionBox().bottom - m_Player.getCollisionBox().top)),*/


			//if (Diff.x <= Diff.y && Diff.x <= Diff.z)
			//{
			//	//Set pos in x by doing Diff * (unitVect(Distance))
			//	m_camera.setPosition(m_camera.getPosition() + glm::vec3(Distance.x * (Diff.x + 5), 0, 0));
			//}
			//else if (Diff.y <= Diff.x && Diff.y <= Diff.z)
			//{
			//	m_camera.setPosition(m_Player.getPosition() + glm::vec3(0, (Distance.y + 5) * Diff.y, 0));
			//}
			//else if (Diff.z <= Diff.x && Diff.z <= Diff.y)
			//{
			//	m_camera.setPosition(m_Player.getPosition() + glm::vec3(0, 0, (Distance.z + 5) * Diff.z));
			//}


					//m_camera.setPosition(m_Player.getPosition());
				}
			}

			/*if (m_CollisionBodies[0->checkCollideWith(rigidBody))
			{
				std::cout << "Colliding" << std::endl;
			}
			else
			{
				std::cout << "Not Colliding" << std::endl;
			}*/

		//if (TestCollision(m_Player, m_sceneReader.m_modelList.at(3), CInfo))
		//{
		//	cout << "Colliding" << endl;

		//	/*float ma = (-2*-2) ;
		//	float mb = (-2*-2) ;

		//	float m = ma + mb;

		//	m_Player.setPosition(m_Player.getPosition() + (CInfo.minTranslateVec + CInfo.penetration * (ma/ m)));
		//	m_sceneReader.m_modelList.at(3).setPosition(m_Player.getPosition() + (CInfo.minTranslateVec + CInfo.penetration * (mb / m)));*/
		//}
		//else
		//{
		//	cout << "Not Colliding" << endl;
		//}
	//}
	//}
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// WORLD
	// Activates World shader
	m_worldShader.use();
	setMatrices(&m_worldShader, glm::mat4(1.0f), m_camera.getView(), m_camera.getProjection());

	m_Player.buffer();
	m_worldShader.setUniform("M", m_Player.m_M);

	//tester.buffer();
	//m_worldShader.setUniform("M", tester.m_M);
	//tester.render();
	
	/*for (int i = 0; i < m_CollisionBodies.size(); i++)
	{
		if (m_CollisionBodies.at(i)->getCollisionShape()->getShapeType() == STATIC_PLANE_PROXYTYPE)
		{
			renderPlane(m_CollisionBodies.at(i));
		}
		else if (m_CollisionBodies.at(i)->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE)
		{
			renderSphere(m_CollisionBodies.at(i));
		}
	}*/

	
	//m_collisionWorld->debugDrawWorld();

	//m_Player.render();
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		// Defines a transformation matrix that does nothing
		glm::mat4 transMat(1.0f);

		// If Model is collectable
		if (m_sceneReader.m_modelList.at(i).isCollectable())
		{
			// Adds the Yoffset to the transformation matrix
			transMat = glm::mat4(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, m_collectYOffset, 0, 1 
			);
		}

		// If Model is not collected
		if (!m_sceneReader.m_modelList.at(i).getCollected())
		{
			// Buffers the Model
			m_sceneReader.m_modelList.at(i).buffer();
			// Sets the Model transformation matrix
			m_worldShader.setUniform("M", m_sceneReader.m_modelList.at(i).m_M * transMat);
			// Renders the Model
			m_sceneReader.m_modelList.at(i).render();
		}
	}

	// Activates FreeType shader

	m_freeType.use();
	// Configures projection
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.f, float(m_windowSize.y)));
	// Renders placeholder text to HUD
	m_pHUD->RenderText(m_freeType.getHandle(), "Placeholder", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
}
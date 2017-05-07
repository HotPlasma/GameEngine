#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"
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

	Bouncer m_collHeight = Bouncer(Bounds(3.0f, 1.0f)); //!< Bouncing value for collectable Y position

	std::shared_ptr<Model> m_pSkybox; //!< World skybox Model

	sf::Clock m_batteryTimer;
	sf::Clock m_levelTimer;
	sf::Clock aiWander;
	int m_iSpareBatteries = 0;
	int m_iBatteryLife = 100;
	int m_iCountdown = 200;
	string m_sLife;
	string m_sTime;
	float aiSpawn = 0;
	glm::vec3 m_aiSpeed; //ai movement speed
	glm::vec3 m_aiRotation; //ai rotation
	sf::SoundBuffer m_aiFootsteps;
	sf::SoundBuffer m_ambientMusic;
	sf::SoundBuffer m_aiScream;
	bool loopSound = true;
	bool aiSearching = true;
	double rotationAngle;
	float movementSpeed = 0.002;
	Model m_Player;

	bool m_bPlayerMoved = false;

	void setLightParams(GLSLProgram *pShader, Camera *camera);

public:

	World(GLFWwindow *pWindow, const sf::Vector2i kWindowSize);

	void initScene(Freetype* pOverlay);

	void input_key(const int kiKey, const int kiAction);
	void input_char(const unsigned int kuiUnicode) {}
	void input_button(const int kiButton, const int kiAction) {}
	void input_scroll(const double kdDelta) {}

	void update(const float kfTimeElapsed);
	void render();
};
#endif
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

class World : public Scene
{
private:

	SceneReader m_sceneReader; // Reads .xml file in order to create world

	bool m_collectGoingUp = true; // Whether collectables are moving up
	float m_collectYOffset = 0.0f; // Y axis offset for collectables
	Bounds m_collectBounds = Bounds(3.0f, 1.0f); // Upper and lower bounds for Y offset

	std::shared_ptr<Model> m_pSkybox; //!< World skybox Model

	sf::Clock m_batteryTimer;
	sf::Clock m_levelTimer;
	int m_iSpareBatteries = 0;
	int m_iBatteryLife = 100;
	int m_iCountdown = 10;
	string m_sLife;
	string m_sTime;
	
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
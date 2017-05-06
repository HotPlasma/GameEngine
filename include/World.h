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

	sf::Clock batteryTimer;
	sf::Clock LevelTimer;
	int spareBatteries = 0;
	int batteryLife = 100;
	int lCountdown = 10;
	string bLife;
	string lTime;
	string extraBatteries;

	std::shared_ptr<Model> m_pSkybox; //!< World skybox Model

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
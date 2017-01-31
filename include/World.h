#ifndef WORLD_H
#define WORLD_H



#include "Texture.h"

#include "scene.h"
#include "Camera.h"
#include "SceneReader.h"

class World : public Scene
{
private:
	int width, height; // Dimentions of window
	glm::vec3 m_CollectableSpeed; // Speed at which a collectable bounces

	unsigned int m_uiBatteriesCollected = 0; // Batteries that have been collected

	GLuint programHandle; // Program context
	glm::mat4 M; // Allows manipulation of each models position, rotation and scale
	void linkMe(GLint vertShader, GLint fragShader); // Connects shaders
	Texture *gTexture; // Holds texture
	Camera FirstPersonView; // Camera which user can control
	GLFWwindow *Window; // The window
	sf::Vector2i MousePos; // Holds mouse cursor position
	SceneReader world; // Reads .xml file in order to create world

public:
	World();
	void initScene();
	void GetMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos);
	void update(float t);
	void ModelUpdate(int index);
	void render();
	void resize(int, int);

};
#endif  

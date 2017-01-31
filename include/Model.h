#pragma once

#include <string>
#include "ModelReader.h"
#include <Bitmap.h>
#include <Texture.h>

using namespace std;

class Model
{
private:
	string sName; // A moniker for each model
	string sFileName; // File location of the obj
	string sTexture; // File location of the texture
	int ModelMaterial; // Which material the model with have. Different integers are different materials.

	glm::vec3 ModelPosition; // Position of Model
	glm::vec3 ModelRotation; // Rotation of Model
	glm::vec3 ModelScale; // Scale of Model

	bool m_bCollectable = false; // is the model a collectable?
	bool m_bCollected = false; // has the collectable been collected?

	GLuint m_textureID; // ID of the texture
	ModelReader* m_modelReader; // Model Reader for the render the model

	GLuint programHandle;
	Texture *gTexture;
	Bitmap bmp = Bitmap::bitmapFromFile("assets/textures/Default.bmp");;

	

	GLuint vboHandles[2];
	GLuint vbo;
	GLuint vaoHandle;

	

public:
	Model(); // Constructor
	Model(string FileLocation, string TextureLocation, glm::vec3 Position, glm::vec3 Rotation, glm::vec3, int MaterialID); // Full constructor
	string GetName(); // Returns moniker
	string GetFileLocation(); // Returns location of obj
	GLuint GetTextureLocation(); // Returns location of texture
	glm::vec3 GetPosition(); // Returns rosition of model
	glm::vec3 GetRotation(); // Returns rotation of model
	glm::vec3 GetScale(); // Returns scale of model
	bool CheckIfCollectable(); // Check if a model is a collectable
	bool GetCollected(); // Check if a collectable has been collected
	int GetMaterial(); // Returns materialID

	void SetCollectable();
	void SetCollected(bool Collected); // Set collected or not collected for a collectable
	void SetName(string newName);
	void SetTexture(GLuint TextureID); // Set texture ID
	void SetFileLocation(string NewLocation); // Set obj location
	void SetTextureLocation(string NewLocation); // Set texture location
	void SetPosition(glm::vec3 NewPosition); // Set model position
	void SetRotation(glm::vec3 NewRotation); // Set model rotation
	void SetScale(glm::vec3 NewScale); // Set model scale
	void SetMaterial(int MaterialID); // Set model MaterialID
	void Buffer();

	vector<float> positionData;
	vector<float> uvData;

	glm::mat4 M;

	void LoadModel(string Model); // Loads in the model to be rendered
	void DrawModel(bool drawWithNormals, bool drawWithTexture); // Draws model


};


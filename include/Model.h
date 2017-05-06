#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "PreHeader.h"

#include "ModelReader.h"

class Model
{
private:
  
	string m_sName; // A moniker for each model
	string m_sFileName; // File location of the obj
	string m_sTexture; // File location of the texture
	int m_iMaterial; // Which material the model with have. Different integers are different materials.

	glm::vec3 m_position; // Position of Model
	glm::vec3 m_rotation; // Rotation of Model
	glm::vec3 m_scale; // Scale of Model
	glm::mat4 m_M; // Transformation matrix

	bool m_bCollectable = false; // is the model a collectable?
	bool m_bCollected = false; // has the collectable been collected?

	bool m_bVisible = true; // is the model visable

	GLuint m_textureID; // ID of the texture
	ModelReader* m_pModelReader; // Model Reader for the render the model

	GLuint m_programHandle;
	Texture *m_pTexture;
	Bitmap m_bmp = Bitmap::bitmapFromFile("assets/textures/Default.bmp");;

	GLuint m_vboHandles[2];
	GLuint m_vbo;
	GLuint m_vaoHandle;

	bool m_b3D = false;

public:

	Model(); // Constructor
	Model(string FileLocation, string TextureLocation, glm::vec3 Position, glm::vec3 Rotation, glm::vec3, int MaterialID, bool b3D); // Full constructor

	std::vector<float> m_positionData;
	std::vector<float> m_normalData;
	std::vector<float> m_uvData;

	void setName(const string ksName) { m_sName = ksName; }
	void setFileLocation(const string ksLocation) { m_sFileName = ksLocation; }
	void setTextureLocation(const string ksLocation) { m_sTexture = ksLocation; }
	void setTextureID(const GLuint kTextureID) { m_textureID = kTextureID; }
	void setPosition(const glm::vec3 kPosition) { m_position = kPosition; }
	void setRotation(const glm::vec3 kRotation) { m_rotation = kRotation; }
	void setScale(const glm::vec3 kScale) { m_scale = kScale; }
	void setM(const glm::mat4 kM) { m_M = kM; }
	void setCollectable(const bool kbCollectable) { m_bCollectable = kbCollectable; }
	void setCollected(const bool kbCollected) { m_bCollected = kbCollected; }
	void setVisible(const bool kbVisibility) { m_bVisible = kbVisibility; }
	void setMaterial(const int kiMaterial) { m_iMaterial = kiMaterial; }
	void set3D(const bool kb3D) { m_b3D = kb3D; }
	
	string getName() { return m_sName; }; // Returns Model name
	string getFileLocation() { return m_sFileName; }; // Returns location of obj
	string getTextureLocation() { return m_sTexture; }; // Returns location of texture
	GLuint getTextureID() { return m_textureID; }; // Returns texture ID
	glm::vec3 getPosition() { return m_position; }; // Returns rosition of model
	glm::vec3 getRotation() { return m_rotation; }; // Returns rotation of model
	glm::vec3 getScale() { return m_scale; }; // Returns scale of model
	glm::mat4 getM() { return m_M; }; // Returns model transformation matrix
	bool isCollectable() { return m_bCollectable; }; // Check if a model is a collectable
	bool isCollected() { return m_bCollected; }; // Check if a collectable has been collected
	bool isVisible() { return m_bVisible; }; // Check if a collectable has been collected
	int getMaterial() { return m_iMaterial; }; // Returns materialID
	bool get3D() { return m_b3D; }

	void loadModel() { m_pModelReader = new ModelReader(m_sFileName); } // Loads in the model to be rendered
	void initModel(); // Draws model

	void render(GLSLProgram* pShader, const glm::mat4 kModel);
};

#endif
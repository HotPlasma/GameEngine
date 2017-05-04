#pragma once

#include "PreHeader.h"

#include "ModelReader.h"

#include "reactphysics3d.h"

using namespace std;


struct CollisionBox
{
	float front;
	float back;
	float left;
	float right;
	float top;
	float bottom;

	glm::vec3 max;
	glm::vec3 min;
	glm::vec3 boundingBox;
};

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

		glm::vec3 m_BoundingBox;

		glm::vec3 m_BoundBoxMax;
		glm::vec3 m_BoundBoxMin;

	public:
		Model(); // Constructor
		Model(string FileLocation, string TextureLocation, glm::vec3 Position, glm::vec3 Rotation, glm::vec3, int MaterialID); // Full constructor
		string getName() { return m_sName; }; // Returns moniker
		string getFileLocation() { return m_sFileName; }; // Returns location of obj
		string getTexFileLocation() { return m_sTexture; }; // Returns file location of texture
		GLuint getTextureLocation() { return m_textureID; }; // Returns location of texture
		glm::vec3 getPosition() { return m_position; }; // Returns rosition of model
		glm::vec3 getRotation() { return m_rotation; }; // Returns rotation of model
		glm::vec3 getScale() { return m_scale; }; // Returns scale of model
		glm::vec3 getCollisionMesh() { return m_BoundingBox; };
		bool isCollectable() { return m_bCollectable; }; // Check if a model is a collectable
		bool getCollected() { return m_bCollected; }; // Check if a collectable has been collected
		bool getVisable() { return m_bVisible; }; // Check if a collectable has been collected
		int getMaterial() { return m_iMaterial; }; // Returns materialID
	
		void setCollectable(bool NewSetting);
		void setCollected(bool bCollected); // Set collected or not collected for a collectable
		void setName(string sNewName);
		void setTexture(GLuint textureID); // Set texture ID
		void setFileLocation(string sNewLocation); // Set obj location
		void setTextureLocation(string sNewLocation); // Set texture location
		void setVisable(bool Visability); // Check if a collectable has been collected
		void setPosition(glm::vec3 newPosition); // Set model position
		void setRotation(glm::vec3 newRotation); // Set model rotation
		void setScale(glm::vec3 newScale); // Set model scale
		void setMaterial(int iMaterialID); // Set model MaterialID
		void buffer();
		CollisionBox getCollisionBox();
		vector<float> m_positionData;
		vector<float> m_uvData;
	
		glm::mat4 m_M;
	
		void loadModel(); // Loads in the model to be rendered
		void initModel(); // Draws model
	
		void render();
};


#pragma once

#include "PreHeader.h"
#include "Model.h"
#include <vector>
#include "Texture.h"
#include <tinyxml2.h>

class SceneReader
{
public:
	// Reads in a xml file and creates models from the data
	SceneReader();
	SceneReader(string filename); // Takes a file name and loads in a scene
	vector<Model> ModelList; // Vector of models which makes up the scene

private:
	vector<GLuint> m_textureID; // Vector of all the textures for models
	Texture *gTexture;
	void ReadSceneFile(string filename); // Function to proccess textfile
};


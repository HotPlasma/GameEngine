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
		SceneReader(string sFilename); // Takes a file name and loads in a scene
		vector<Model> m_modelList; // Vector of models which makes up the scene
	
	private:
		vector<GLuint> m_textureID; // Vector of all the textures for models
		Texture *g_pTexture;
		void readSceneFile(string sFilename); // Function to proccess textfile
};


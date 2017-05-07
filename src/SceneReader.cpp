#include "SceneReader.h"

SceneReader::SceneReader() {}

SceneReader::SceneReader(string sFilename)
{
	// Reads the input file path
	readSceneFile(sFilename);

	// Create a new textureloader
	//tl = new TextureLoader(); 
}

void SceneReader::readSceneFile(string sFilename)
{
	// Declares new XML Document
	tinyxml2::XMLDocument doc; 
	// Loads XML Document from given file path
	doc.LoadFile(sFilename.c_str());

	// For every tier 1 Node
	for (tinyxml2::XMLNode* iNode = doc.FirstChild(); iNode != NULL; iNode = iNode->NextSibling())
	{
		// If the Node is <Scene>
		if (strcmp(iNode->Value(), "Scene") == 0)
		{
			// For every tier 2 Node
			for (tinyxml2::XMLNode* iNode2 = iNode->FirstChild(); iNode2 != NULL; iNode2 = iNode2->NextSibling())
			{
				// If the Node is <Object>
				if (strcmp(iNode2->Value(), "Object") == 0)
				{
					// Temporary Model to be pushed into Model vector
					Model newModel;

					// For every tier 3 Node
					for (tinyxml2::XMLNode* iNode3 = iNode2->FirstChild(); iNode3 != NULL; iNode3 = iNode3->NextSibling())
					{
						// Sets the Node name to a string stream
						std::istringstream iss(iNode3->ToElement()->GetText());
						// String to hold data coming out of the stringstream
						string sData; 

						// If the Node is <Name>
						if (strcmp(iNode3->Value(), "Name") == 0)
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// newModel name set to the data string
							newModel.setName(sData);
						}
						// If the Node is <OBJLocation>
						if (strcmp(iNode3->Value(), "OBJLocation") == 0)
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// newModel .obj file path set to the data string
							newModel.setFileLocation(sData);
						}
						// If the Node is <TexLocation>
						if (strcmp(iNode3->Value(), "TexLocation") == 0)
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// Make the textureID vector 1 bigger
							m_textureID.resize(m_textureID.size() + 1);

							// Set texture to end of vector
							//	tl->LoadBMP(data, m_textureID[m_textureID.size() - 1], true); 

							// newModel .bmp file path set to the data string
							newModel.setTextureLocation(sData);
						}
						// If the Node is <Translation>
						if (strcmp(iNode3->Value(), "Translation") == 0) // Read in 3 values for the position of the model
						{
							// Declares a new position vector
							glm::vec3 position;

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Position X set to the data string bound to a float
							position.x = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Position Y set to the data string bound to a float
							position.y = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Position Z set to the data string bound to a float
							position.z = stof(sData);

							// newModel position set to the position vector
							newModel.setPosition(position);
						}
						// If the Node is <Rotation>
						if (strcmp(iNode3->Value(), "Rotation") == 0) // Read in 3 values for the rotation of the model
						{
							// Declares a new rotation vector
							glm::vec3 rotation;

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Rotation X set to the data string bound to a float
							rotation.x = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Rotation Y set to the data string bound to a float
							rotation.y = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Rotation Z set to the data string bound to a float
							rotation.z = stof(sData);

							// newModel rotation set to the rotation vector
							newModel.setRotation(rotation);
						}
						// If the Node is <Scale>
						if (strcmp(iNode3->Value(), "Scale") == 0) // Read in 3 values for the scale of the model
						{
							// Declares a new scale vector
							glm::vec3 scale;

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Scale X set to the data string bound to a float
							scale.x = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Scale Y set to the data string bound to a float
							scale.y = stof(sData);

							// Binds current data in the stringstream to a string
							iss >> sData;
							// Scale Z set to the data string bound to a float
							scale.z = stof(sData);

							// newModel scale set to the scale vector
							newModel.setScale(scale);
						}
						// If the Node is <Material>
						if (strcmp(iNode3->Value(), "Material") == 0) // Assign a material for the model
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// newModel material set to the data string bound to an integer
							newModel.setMaterial(stoi(sData));
						}
						// If the Node is <Collectable>
						if (strcmp(iNode3->Value(), "Collectable") == 0) // Check if model is a collectable
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// If data string contains "true"
							if (sData == "true")
							{
								newModel.setCollectable(true);
							}
							else
							{
								newModel.setCollectable(false);
							}
						}
						// If the Node is <AI>
						if (strcmp(iNode3->Value(), "Ai") == 0) // Check if model is a collectable
						{
							// Binds current data in the stringstream to a string
							iss >> sData;

							// If data string contains "true"
							if (sData == "true")
							{
								newModel.setAI(true);
							}
							else
							{
								newModel.setAI(false);
							}
						}
					
					}
					newModel.set3D(true);
					// Pushes newModel onto the vector of models
					m_modelList.push_back(newModel); 
				}
			}
		}
	}

	// For every model
	for (int i = 0; i < m_modelList.size(); i++)
	{
		// Loads Model so it's ready for drawing
		m_modelList[i].loadModel();
	}
}

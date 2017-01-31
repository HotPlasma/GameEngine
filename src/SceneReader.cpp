#include "PreHeader.h"
#include "SceneReader.h"

SceneReader::SceneReader()
{

}

SceneReader::SceneReader(string filename)
{
	ReadSceneFile(filename); // Send file to be read
	//tl = new TextureLoader(); // Create a new textureloader
}

void SceneReader::ReadSceneFile(string filename)
{

	Model Temp; // Temperary model to be pushed into Scene Vector
	string sData; // Current sData being read


	tinyxml2::XMLDocument doc; // XML document

	doc.LoadFile(filename.c_str()); // Load document

	for (tinyxml2::XMLNode* iNode = doc.FirstChild(); iNode != NULL; iNode = iNode->NextSibling())
	{
		if (strcmp(iNode->Value(), "Scene") == 0) // For each scene
		{
			for (tinyxml2::XMLNode* iNode2 = iNode->FirstChild(); iNode2 != NULL; iNode2 = iNode2->NextSibling())
			{
				if (strcmp(iNode2->Value(), "Object") == 0) // For each object within a scene
				{
					for (tinyxml2::XMLNode* iNode3 = iNode2->FirstChild(); iNode3 != NULL; iNode3 = iNode3->NextSibling())
					{
						istringstream iss(iNode3->ToElement()->GetText());
						if (strcmp(iNode3->Value(), "Name") == 0) // Assign name
						{

							iss >> sData;
							Temp.SetName(sData);
						}
						if (strcmp(iNode3->Value(), "OBJLocation") == 0) // Assign .obj file location
						{

							iss >> sData;
							Temp.SetFileLocation(sData);
						}
						if (strcmp(iNode3->Value(), "TexLocation") == 0) // Assign .bmp texture file location
						{
							iss >> sData;
							m_textureID.resize(m_textureID.size() + 1); // Make the textureID vector 1 bigger
																		// Set texture to end of vector
																		//	tl->LoadBMP(data, m_textureID[m_textureID.size() - 1], true); 

							Temp.SetTextureLocation(sData);
						}
						if (strcmp(iNode3->Value(), "Translation") == 0) // Read in 3 values for the position of the model
						{
							glm::vec3 Position;
							iss >> sData;
							Position.x = stof(sData);
							iss >> sData;
							Position.y = stof(sData);
							iss >> sData;
							Position.z = stof(sData);
							Temp.SetPosition(Position);
						}
						if (strcmp(iNode3->Value(), "Rotation") == 0) // Read in 3 values for the rotation of the model
						{

							glm::vec3 Rotation;
							iss >> sData;
							Rotation.x = stof(sData);
							iss >> sData;
							Rotation.y = stof(sData);
							iss >> sData;
							Rotation.z = stof(sData);
							Temp.SetRotation(Rotation);
						}
						if (strcmp(iNode3->Value(), "Scale") == 0) // Read in 3 values for the scale of the model
						{

							glm::vec3 Scale;
							iss >> sData;
							Scale.x = stof(sData);
							iss >> sData;
							Scale.y = stof(sData);
							iss >> sData;
							Scale.z = stof(sData);
							Temp.SetScale(Scale);
						}
						if (strcmp(iNode3->Value(), "Material") == 0) // Assign a material for the model
						{

							iss >> sData;
							Temp.SetMaterial(stoi(sData));
						}
						if (strcmp(iNode3->Value(), "Collectable") == 0) // Check if model is a collectable
						{

							iss >> sData;
							if (sData == "true")
							{
								Temp.SetCollectable();
							}
						}
					}
					ModelList.push_back(Temp); // Push to vector of models 
				}
			}
		}
	}

	for (int i = 0; i < ModelList.size(); i++)
	{
		ModelList[i].LoadModel(ModelList[i].GetFileLocation()); // Load in all models to be ready for drawing
	}
}

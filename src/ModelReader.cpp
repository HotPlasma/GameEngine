#include "PreHeader.h"
#include "ModelReader.h"



ModelReader::ModelReader(string filename)
{
	ReadModelObjData(filename);

	// Expand the data suitable for lDrawArrays()
	CreateExpandedVertices();
	CreateExpandedNormals();
	CreateExpandedTextureCoordinates();
}

ModelReader::~ModelReader(void)
{

}

void ModelReader::ReadModelObjData(string filename)
{
	std::fstream modelfile(filename, std::ios_base::in);

	if(!modelfile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument( "File not found" );
		return;	// Throws error if file not found
	}

	string line;
	string token = "";
	while (getline(modelfile, line))
	{
		istringstream iss(line);

		// Process the line
		token = "";

		iss >> token; // Read to next whitespace
	
		if (token == "#")
		{
			// Ignore rest of line
		}
		else if (token == "g")
		{
			// Read the model name
			iss >> m_modelName;
			// Ignore rest of line
		}
		else if (token == "v") // Read in vertex
		{
			ProcessVertexLine(iss);
		}
		else if (token == "vn") // Read in vertex normal
		{
			ProcessVertexNormalLine(iss);
		}
		else if (token == "vt") // Read in texture point
		{
			ProcessVertexTextureLine(iss);
		}
		else if (token == "f") // Read in face
		{
			ProcessFaceLine(iss);
		}
		else
		{
			// Ignore any line without these qualifers
		}
	}
	modelfile.close();

}

void ModelReader::ProcessVertexLine(istringstream& iss)
{

	// Get all 3 verticies
	float fVertex;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertex;

		m_vertices.push_back(fVertex); // Push to vector of vertices
	}

}

void ModelReader::ProcessVertexNormalLine(istringstream& iss)
{
	// Get all vertices normals
	float fVertexNormal;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertexNormal;

		m_vertexNormals.push_back(fVertexNormal); // Push to vertex normal's vector
	}
}

void ModelReader::ProcessVertexTextureLine(istringstream& iss)
{
	// Get vertex texture lines
	float fVertexTextureLine;

	for (int i = 0; i < 2; i++)
	{
		iss >> fVertexTextureLine;

		m_vertexTextureCoordinates.push_back(fVertexTextureLine); // Push to vector of vertex texture lines
	}
}


void ModelReader::ProcessFaceLine(istringstream& iss)
{
	// Get all face lines
	int iFaces;
	static const int forwardSlash = 0x2F;

	for (int i = 0; i < 3; i++)
	{
		iss >> iFaces;
		m_faceVertexIndices.push_back(iFaces - 1);
		// Now look for a slash
		int lookAhead = iss.peek();
		if (lookAhead == forwardSlash)
		{
			// If its a slash get rid of it
			int discard = iss.get();
			// Check for another slash after
			lookAhead = iss.peek();

			if (lookAhead == forwardSlash)
			{
				// If it is then get rid of it
				discard = iss.get();

				// Get normal

				int iNormal = 0;
				iss >> iNormal;

				m_faceNormalIndices.push_back(iNormal - 1);
			}
			else
			{
				// If only one slash then its a texture index
				int iTexture;
				iss >> iTexture;
				m_faceTextureIndices.push_back(iTexture - 1);
				// Discard slash
				discard = iss.get();
				// Get normal
				int iNormal;
				iss >> iNormal;
				m_faceNormalIndices.push_back(iNormal - 1);

			}
		}
		else
		{
			// Discard unimportant data or whitespace
		}
	}
}


void ModelReader::CreateExpandedVertices()
{
	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin() ; it != m_faceVertexIndices.end(); ++it)
	{
		int vertexNumber = (*it);
		// 3 floats per triangular face
		
		int a;
		a = (*it) * 3;

		for (int i = 0; i < 3; i++)
		{

			float v = m_vertices[a + i];


			m_vertexTriplets.push_back(v);
		}	
	}
}
void ModelReader::CreateExpandedNormals()
{
	// Creates a list of normal triplets

	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin() ; it != m_faceNormalIndices.end(); ++it)
	{
	// Loop through and aquire 
		int vertexNormalNumber = (*it);

		int a;
		a = (*it) * 3;

		for (int i = 0; i < 3; i++)
		{

			if ((a + i) >= (int)m_vertexNormals.size())
			{

			}
			else
			{
				float v = m_vertexNormals[a + i];
			

				m_vertexNormalTriplets.push_back(v);
			}
		}	

	}
}
void ModelReader::CreateExpandedTextureCoordinates()
{
	// Get workable texture coordinates

	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin() ; it != m_faceTextureIndices.end(); ++it)
	{
		int textureNumber = (*it);

		int a = textureNumber * 2;

		for (int i = 0; i < 2; i++)
		{
			m_vertexTexturePairs.push_back(m_vertexTextureCoordinates[a + i]);
		}
	}

}


// Get methods give access to the vector data for rendering

vector<float>& ModelReader::GetVertices()
{
	return m_vertexTriplets;
}
vector<float>& ModelReader::GetNormals()
{
	return m_vertexNormalTriplets;
}
vector<float>& ModelReader::GetTextureCoordinates()
{
	return m_vertexTexturePairs;
}
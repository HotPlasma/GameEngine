#include "ModelReader.h"

ModelReader::ModelReader(string filename)
{
	readModelObjData(filename);

	// Expand the data suitable for lDrawArrays()
	createExpandedVertices();
	createExpandedNormals();
	createExpandedTextureCoordinates();
}

ModelReader::~ModelReader(void)
{

}

void ModelReader::readModelObjData(string filename)
{
	std::fstream modelfile(filename, std::ios_base::in);

	if(!modelfile.is_open())
	{
		std::cerr << "File " << filename << " not found.";
		DebugBreak();
		throw std::invalid_argument( "File not found" );
		return;	// Throws error if file not found
	}

	string sLine;
	string sToken = "";
	while (getline(modelfile, sLine))
	{
		std::istringstream iss(sLine);

		// Process the line
		sToken = "";

		iss >> sToken; // Read to next whitespace
	
		if (sToken == "#")
		{
			// Ignore rest of line
		}
		else if (sToken == "g")
		{
			// Read the model name
			iss >> m_modelName;
			// Ignore rest of line
		}
		else if (sToken == "v") // Read in vertex
		{
			processVertexLine(iss);
		}
		else if (sToken == "vn") // Read in vertex normal
		{
			processVertexNormalLine(iss);
		}
		else if (sToken == "vt") // Read in texture point
		{
			processVertexTextureLine(iss);
		}
		else if (sToken == "f") // Read in face
		{
			processFaceLine(iss);
		}
		else
		{
			// Ignore any line without these qualifers
		}
	}
	modelfile.close();

}

void ModelReader::processVertexLine(std::istringstream& iss)
{
	// Get all 3 verticies
	float fVertex;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertex;

		m_vertices.push_back(fVertex); // Push to vector of vertices
	}
}
void ModelReader::processVertexNormalLine(std::istringstream& iss)
{
	// Get all vertices normals
	float fVertexNormal;

	for (int i = 0; i < 3; i++)
	{
		iss >> fVertexNormal;

		m_vertexNormals.push_back(fVertexNormal); // Push to vertex normal's vector
	}
}
void ModelReader::processVertexTextureLine(std::istringstream& iss)
{
	// Get vertex texture lines
	float fVertexTextureLine;

	for (int i = 0; i < 2; i++)
	{
		iss >> fVertexTextureLine;

		m_vertexTextureCoordinates.push_back(fVertexTextureLine); // Push to vector of vertex texture lines
	}
}
void ModelReader::processFaceLine(std::istringstream& iss)
{
	// Get all face lines
	int iFaces;
	static const int kiForwardSlash = 0x2F;

	for (int i = 0; i < 3; i++)
	{
		iss >> iFaces;
		m_faceVertexIndices.push_back(iFaces - 1);
		// Now look for a slash
		int iLookAhead = iss.peek();
		if (iLookAhead == kiForwardSlash)
		{
			// If its a slash get rid of it
			int iDiscard = iss.get();
			// Check for another slash after
			iLookAhead = iss.peek();

			if (iLookAhead == kiForwardSlash)
			{
				// If it is then get rid of it
				iDiscard = iss.get();

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
				iDiscard = iss.get();
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

void ModelReader::createExpandedVertices()
{
	for (std::vector<unsigned int>::iterator it = m_faceVertexIndices.begin() ; it != m_faceVertexIndices.end(); ++it)
	{
		int iVertexNumber = (*it);

		// 3 floats per triangular face
		int iA;
		iA = iVertexNumber * 3;

		for (int i = 0; i < 3; i++)
		{
			float fV = m_vertices[iA + i];

			m_vertexTriplets.push_back(fV);
		}	
	}
}
void ModelReader::createExpandedNormals()
{
	// Creates a list of normal triplets
	for (std::vector<unsigned int>::iterator it = m_faceNormalIndices.begin() ; it != m_faceNormalIndices.end(); ++it)
	{
		// Loop through and aquire 
		int iVertexNormalNumber = (*it);

		int iA;
		iA = iVertexNormalNumber * 3;

		for (int i = 0; i < 3; i++)
		{
			if ((iA + i) >= (int)m_vertexNormals.size())
			{
				// Nothing
			}
			else
			{
				float fV = m_vertexNormals[iA + i];
			
				m_vertexNormalTriplets.push_back(fV);
			}
		}
	}
}
void ModelReader::createExpandedTextureCoordinates()
{
	// Get workable texture coordinates
	for (std::vector<unsigned int>::iterator it = m_faceTextureIndices.begin() ; it != m_faceTextureIndices.end(); ++it)
	{
		int iTextureNumber = (*it);

		int iA = iTextureNumber * 2;

		for (int i = 0; i < 2; i++)
		{
			m_vertexTexturePairs.push_back(m_vertexTextureCoordinates[iA + i]);
		}
	}
}
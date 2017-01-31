#pragma once

#include "PreHeader.h"

using namespace std;

class ModelReader
{
	private:
		
		static const int forwardSlash = 0x2F; // Forward slash

		// For storing read data from the obj file

		vector<float> m_vertices;					// v
		vector<float> m_vertexNormals;				// vn
		vector<float> m_vertexTextureCoordinates;	// vt only U V

		vector<unsigned int> m_faceVertexIndices;
		vector<unsigned int> m_faceTextureIndices;
		vector<unsigned int> m_faceNormalIndices;

		string m_modelName;

		vector<float> m_vertexTriplets;	
		vector<float> m_vertexNormalTriplets; 
		vector<float> m_vertexTexturePairs;

	public:
		/*ModelReader();*/
		ModelReader(string filename);
		~ModelReader(void);

		void ModelReader::ReadModelObjData(string filename);

		void ProcessVertexLine(istringstream& iss);
		void ProcessVertexNormalLine(istringstream& iss);
		void ProcessVertexTextureLine(istringstream& iss);
		void ProcessFaceLine(istringstream& iss);


		void CreateExpandedVertices();
		void CreateExpandedNormals();
		void CreateExpandedTextureCoordinates();

		vector<float>& GetVertices();
		vector<float>& GetNormals();
		vector<float>& GetTextureCoordinates();

};


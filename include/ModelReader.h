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
		ModelReader(string sFilename);
		~ModelReader(void);
	
		void readModelObjData(string sFilename);
	
		void processVertexLine(istringstream& iss);
		void processVertexNormalLine(istringstream& iss);
		void processVertexTextureLine(istringstream& iss);
		void processFaceLine(istringstream& iss);
	
		void createExpandedVertices();
		void createExpandedNormals();
		void createExpandedTextureCoordinates();
	
		vector<float>& getVertices() { return m_vertexTriplets; };
		vector<float>& getNormals() { return m_vertexNormalTriplets; };
		vector<float>& getTextureCoordinates() { return m_vertexTexturePairs; };
};


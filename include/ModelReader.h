#pragma once
#ifndef MODELREADER_H
#define MODELREADER_H

#include "PreHeader.h"

class ModelReader
{
private:
	
	static const int forwardSlash = 0x2F; // Forward slash

	// For storing read data from the obj file
	std::vector<float> m_vertices;					// v
	std::vector<float> m_vertexNormals;				// vn
	std::vector<float> m_vertexTextureCoordinates;	// vt only U V

	std::vector<unsigned int> m_faceVertexIndices;
	std::vector<unsigned int> m_faceTextureIndices;
	std::vector<unsigned int> m_faceNormalIndices;

	string m_modelName;

	std::vector<float> m_vertexTriplets;
	std::vector<float> m_vertexNormalTriplets;
	std::vector<float> m_vertexTexturePairs;

public:

	ModelReader(string sFilename);
	~ModelReader(void);

	void readModelObjData(string sFilename);

	void processVertexLine(std::istringstream& iss);
	void processVertexNormalLine(std::istringstream& iss);
	void processVertexTextureLine(std::istringstream& iss);
	void processFaceLine(std::istringstream& iss);

	void createExpandedVertices();
	void createExpandedNormals();
	void createExpandedTextureCoordinates();

	std::vector<float>& getVertices() { return m_vertexTriplets; };
	std::vector<float>& getNormals() { return m_vertexNormalTriplets; };
	std::vector<float>& getTextureCoordinates() { return m_vertexTexturePairs; };
};

#endif
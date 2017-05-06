#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

// Class for storing upper and lower bounds
class Bounds
{
public:

	// Constructor
	Bounds(const float kfUpper, const float kfLower)
	{
		// Sets members to appropriate input
		m_fUpper = kfUpper;
		m_fLower = kfLower;
	}

	float upper() { return m_fUpper; } // Returns upper bound
	float lower() { return m_fLower; } // Returns lower bound

private:

	float m_fUpper; // Upper bound
	float m_fLower; // Lower bound
};

static void closeProgram()
{
	// Terminates GLFW and closes the program
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

static bool fileExists(const std::string ksFilePath)
{
	// Declares a file stream with the input file path
	std::ifstream file(ksFilePath.c_str());

	// Returns whether the file exists or not
	return file.good();
}

#endif
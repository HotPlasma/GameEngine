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

// Class that stores a value that bounces up and down in bounds
class Bouncer
{
public:

	// Constructor
	Bouncer(const Bounds bounds)
	{
		// Sets members to appropriate input
		m_bounds = bounds; // Upper and lower bounds for value
	}

	float value() { return m_fCurrValue; } // Returns value

	void update(const float kfMovement) // Updates Bouncer with a set value change
	{ 
		// If value is moving up and value is greater than upper bound
		if (m_goingUp && m_fCurrValue >= m_bounds.upper())
		{
			// Move collectable down
			m_goingUp = false;
		}

		// If collectables are moving down and offset is less than lower bound
		if (!m_goingUp && m_fCurrValue <= m_bounds.lower())
		{
			// Move collectable up
			m_goingUp = true;
		}

		// Increments offset up or down
		if (m_goingUp) { m_fCurrValue += kfMovement; }
		else if (!m_goingUp) { m_fCurrValue -= kfMovement; }

		// Clamps offset to bounds
		m_fCurrValue = glm::clamp(m_fCurrValue, m_bounds.lower(), m_bounds.upper());

	} 

private:

	bool m_goingUp = true; // Whether the value is going up
	float m_fCurrValue = 0.0f; // Current value
	Bounds m_bounds = Bounds(0.0f, 1.0f); // Upper and lower bounds for value bounce
};

// Class for timed text display
class PopUp
{
public:

	PopUp() {}

	// Decrements the countdown with input
	void countdown(const float kfTimeElapsed) 
	{ 
		m_fCountdown -= kfTimeElapsed;
		if (m_fCountdown <= 0.0f) { m_fCountdown = 0.0f; } // Clamps to 0 or higher
	}

	// Creates a pop up
	void newPopUp(const std::string ksText, const float kfDuration)
	{
		m_sText = ksText;
		m_fCountdown = kfDuration;
	}

	// Sets countdown
	void setCountdown(const float kfCountdown) { m_fCountdown = kfCountdown; }
	// Returns countdown
	float getCountdown() { return m_fCountdown; }

	// Sets Text
	void setText(const std::string ksText) { m_sText = ksText; }
	// Returns Text
	std::string getText() { return m_sText; }

private:

	// Float countdown value
	float m_fCountdown = 0.0;
	// String text value
	std::string m_sText = "";
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
#include "PreHeader.h"

#include "World.h"

#include <cstdio>
#include <cstdlib>

#include "ModelReader.h"


using std::string;
using std::ifstream;

World::World(sf::Vector2i windowSize)
{
	m_windowSize = windowSize;
}

void World::initScene()
{
	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	ifstream inFile("shaders/shader.vert");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader/shader.vert\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string sCodeStr(code.str());

	// Create the shader object
	GLuint vertShader = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = { sCodeStr.c_str() };
	gl::ShaderSource(vertShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(vertShader);

	// Check compilation status
	GLint result;
	gl::GetShaderiv(vertShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Vertex shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(vertShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(vertShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shader code here
	ifstream fragFile("shaders/shader.frag");
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader/shader.frag\n");
		exit(1);
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	sCodeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (0 == fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = sCodeStr.c_str();
	gl::ShaderSource(fragShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(fragShader);

	// Check compilation status
	gl::GetShaderiv(fragShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Fragment shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(fragShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	linkMe(vertShader, fragShader);

	m_sceneReader = SceneReader("assets/scenes/Scene.xml");

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
		{
			m_sceneReader.m_modelList[i].drawModel();
		}
		/*world.ModelList[i].DrawModel(true, true);*/
	}

}

void World::setMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos)
{
	m_pWindow = Gwindow;
	m_mousePos = mousepos;
}

void World::linkMe(GLint vertShader, GLint fragShader)
{
	// Create the program object
	m_programHandle = gl::CreateProgram();
	if (0 == m_programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}

	// Attach the shaders to the program object
	gl::AttachShader(m_programHandle, vertShader);
	gl::AttachShader(m_programHandle, fragShader);

	// Link the program
	gl::LinkProgram(m_programHandle);

	// Check for successful linking
	GLint status;
	gl::GetProgramiv(m_programHandle, gl::LINK_STATUS, &status);
	if (FALSE == status) {

		fprintf(stderr, "Failed to link shader program!\n");

		GLint logLen;
		gl::GetProgramiv(m_programHandle, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetProgramInfoLog(m_programHandle, logLen, &written, log);

			fprintf(stderr, "Program log: \n%s", log);

			free(log);
		}
	}
	else 
	{
		gl::UseProgram(m_programHandle);
	}
}

void World::update(float t)
{

	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		if (m_sceneReader.m_modelList.at(i).isCollectable()) // check if collectable
		{
			if (!m_sceneReader.m_modelList.at(i).getCollected()) // if collectable then slowly rotate and bob up and down
			{
				if (m_sceneReader.m_modelList.at(i).getPosition().y > -7)
				{
					m_collectableSpeed = glm::vec3(0, -0.03, 0);
				}
				else if (m_sceneReader.m_modelList.at(i).getPosition().y < -10)
				{
					m_collectableSpeed = glm::vec3(0, 0.03, 0);
				}
				m_sceneReader.m_modelList.at(i).setPosition(m_sceneReader.m_modelList.at(i).getPosition() + m_collectableSpeed);
				m_sceneReader.m_modelList.at(i).setRotation(glm::vec3(45, m_sceneReader.m_modelList.at(i).getRotation().y + 1, m_sceneReader.m_modelList.at(i).getRotation().z));

				glm::vec3 Distance = m_camera.getPosition() - m_sceneReader.m_modelList.at(i).getPosition(); // Work out distance between robot and a collectable

				if (sqrtf(powf(Distance.x, 2.0f) + powf(Distance.z, 2.0f)) < 5) // If robot collides with a collectable mark it as collected and stop drawing it
				{
					m_sceneReader.m_modelList.at(i).setCollected(true);
					m_uiBatteriesCollected++;
				}
			}
		}
	}

	// Creates camera and view using MVP

	// Identity matrix

	// Changing M effects model in scene


	 // Allows first person view changing with mouse movement
	sf::Vector2i windowOrigin(m_windowSize.x * 0.5, m_windowSize.y * 0.5); // Middle of the screen

	float fYAngle = (windowOrigin - m_mousePos).x / 1000.0f;
	float fZAngle = (windowOrigin - m_mousePos).y / 1000.0f;

	m_camera.processUserInput(fYAngle, fZAngle); // Send mouse position data to be processed in order to move camera

	glm::mat4 V = glm::lookAt(glm::vec3(m_camera.getPosition().x, m_camera.getPosition().y, m_camera.getPosition().z), // Camera position
		glm::vec3(m_camera.getView().x, m_camera.getView().y, m_camera.getView().z), // Looking at
		glm::vec3(0, 1, 0)); // Up

	//std::cout << "X: " << FirstPersonView.GetCameraPos().x << " Y: " << FirstPersonView.GetCameraPos().y << " Z: " << FirstPersonView.GetCameraPos().z << std::endl;

	glm::mat4 P = glm::perspective(60.0f, 1.0f, 1.f, 500.f); // Sets FOV and vision culls

	// Send data to shader for processing

	GLuint viewMatrixID = gl::GetUniformLocation(m_programHandle, "mView");
	GLuint projectionMatrixID = gl::GetUniformLocation(m_programHandle, "mProjection");

	
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, glm::value_ptr(V));
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, glm::value_ptr(P));
}

void World::modelUpdate(int index)
{
	GLuint modelMatrixID = gl::GetUniformLocation(m_programHandle, "mModel");
	gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, glm::value_ptr(m_sceneReader.m_modelList.at(index).m_M));
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Render all models in current scene
	for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	{
		m_sceneReader.m_modelList.at(i).buffer();
		modelUpdate(i);
		gl::DrawArrays(gl::TRIANGLES, 0, m_sceneReader.m_modelList.at(i).m_positionData.size());
	}
}
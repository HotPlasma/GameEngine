#include "PreHeader.h"

#include "World.h"

#include <cstdio>
#include <cstdlib>

#include "ModelReader.h"


using std::string;
using std::ifstream;

World::World() { }

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
	string codeStr(code.str());

	// Create the shader object
	GLuint vertShader = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = { codeStr.c_str() };
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
	codeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (0 == fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
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

	world = SceneReader("assets/scenes/Scene.xml");

	for (int i = 0; i < world.ModelList.size(); i++)
	{
		if (!world.ModelList.at(i).GetCollected()) // Draw all items except collected collectables
		{
			world.ModelList[i].DrawModel(true, true);
		}
		/*world.ModelList[i].DrawModel(true, true);*/
	}

}

void World::GetMousePos(GLFWwindow *Gwindow, sf::Vector2i mousepos)
{
	Window = Gwindow;
	MousePos = mousepos;

}

void World::linkMe(GLint vertShader, GLint fragShader)
{


	// Create the program object
	programHandle = gl::CreateProgram();
	if (0 == programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}


	// Attach the shaders to the program object
	gl::AttachShader(programHandle, vertShader);
	gl::AttachShader(programHandle, fragShader);

	// Link the program
	gl::LinkProgram(programHandle);

	// Check for successful linking
	GLint status;
	gl::GetProgramiv(programHandle, gl::LINK_STATUS, &status);
	if (FALSE == status) {

		fprintf(stderr, "Failed to link shader program!\n");

		GLint logLen;
		gl::GetProgramiv(programHandle, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetProgramInfoLog(programHandle, logLen, &written, log);

			fprintf(stderr, "Program log: \n%s", log);

			free(log);
		}
	}
	else {
		gl::UseProgram(programHandle);
	}

}

void World::update(float t)
{

	for (int i = 0; i < world.ModelList.size(); i++)
	{
		if (world.ModelList.at(i).CheckIfCollectable()) // check if collectable
		{
			if (!world.ModelList.at(i).GetCollected()) // if collectable then slowly rotate and bob up and down
			{
				if (world.ModelList.at(i).GetPosition().y > -7)
				{
					m_CollectableSpeed = glm::vec3(0, -0.03, 0);
				}
				else if (world.ModelList.at(i).GetPosition().y < -10)
				{
					m_CollectableSpeed = glm::vec3(0, 0.03, 0);
				}
				world.ModelList.at(i).SetPosition(world.ModelList.at(i).GetPosition() + m_CollectableSpeed);
				world.ModelList.at(i).SetRotation(glm::vec3(45, world.ModelList.at(i).GetRotation().y + 1, world.ModelList.at(i).GetRotation().z));

				glm::vec3 Distance = FirstPersonView.GetCameraPos() - world.ModelList.at(i).GetPosition(); // Work out distance between robot and a collectable

				if (sqrtf(powf(Distance.x, 2.0f) + powf(Distance.z, 2.0f)) < 5) // If robot collides with a collectable mark it as collected and stop drawing it
				{
					world.ModelList.at(i).SetCollected(true);
					m_uiBatteriesCollected++;
				}
			}
		}
	}

	// Creates camera and view using MVP

	// Identity matrix

	// Changing M effects model in scene


	 // Allows first person view changing with mouse movement
	sf::Vector2i WindowOrigin(1920 / 2, 1080 / 2); // Middle of the screen

	float yAngle = (WindowOrigin - MousePos).x / 1000.0f;
	float zAngle = (WindowOrigin - MousePos).y / 1000.0f;

	FirstPersonView.ProcessUserInput(yAngle, zAngle); // Send mouse position data to be processed in order to move camera

	glm::mat4 V = glm::lookAt(glm::vec3(FirstPersonView.GetCameraPos().x, FirstPersonView.GetCameraPos().y, FirstPersonView.GetCameraPos().z), // Camera position
		glm::vec3(FirstPersonView.GetCameraView().x, FirstPersonView.GetCameraView().y, FirstPersonView.GetCameraView().z), // Looking at
		glm::vec3(0, 1, 0)); // Up

	//std::cout << "X: " << FirstPersonView.GetCameraPos().x << " Y: " << FirstPersonView.GetCameraPos().y << " Z: " << FirstPersonView.GetCameraPos().z << std::endl;


	glm::mat4 P = glm::perspective(60.0f, 1.0f, 1.f, 500.f); // Sets FOV and vision culls

	// Send data to shader for processing


	GLuint viewMatrixID = gl::GetUniformLocation(programHandle, "mView");
	GLuint projectionMatrixID = gl::GetUniformLocation(programHandle, "mProjection");

	
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, glm::value_ptr(V));
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, glm::value_ptr(P));
}

void World::ModelUpdate(int index)
{
	GLuint modelMatrixID = gl::GetUniformLocation(programHandle, "mModel");
	gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, glm::value_ptr(world.ModelList.at(index).M));
}

void World::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	
	// Render all models in current scene
	for (int i = 0; i < world.ModelList.size(); i++)
	{
		world.ModelList.at(i).Buffer();
		ModelUpdate(i);
		gl::DrawArrays(gl::TRIANGLES, 0, world.ModelList.at(i).positionData.size());
		
	}

}

void World::resize(int w, int h)
{
	gl::Viewport(0, 0, w, h);
}

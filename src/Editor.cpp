
#include "Editor.h"

// Constructor
Editor::Editor(const sf::Vector2i kWindowSize)
{
	// Assigns input to appropriate member
	m_windowSize = kWindowSize;
}

// Void: Links vert and frag shaders into a glslprogram
void Editor::linkMe(const GLint kVertShader, const GLint kFragShader)
{
	// Create the program object
	m_programHandle = gl::CreateProgram();
	if (0 == m_programHandle)
	{
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}

	// Attach the shaders to the program object
	gl::AttachShader(m_programHandle, kVertShader);
	gl::AttachShader(m_programHandle, kFragShader);

	// Link the program
	gl::LinkProgram(m_programHandle);

	// Check for successful linking
	GLint status;
	gl::GetProgramiv(m_programHandle, gl::LINK_STATUS, &status);
	if (FALSE == status) 
	{
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

// Void: Initialises the Editor Scene
void Editor::initScene(GLFWwindow* pWindow)
{
	// Window pointer added to member
	m_pWindow = pWindow;

	// Sets the cursor to be hidden
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Stops rendered models from being transparent
	gl::Enable(gl::DEPTH_TEST);

	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	ifstream inFile("shaders/shader.vert");
	if (!inFile) 
	{
		fprintf(stderr, "Error opening file: shader/shader.vert\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string sCodeStr(code.str());

	// Create the shader object
	GLuint vertShader = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertShader) 
	{
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
	if (FALSE == result) 
	{
		fprintf(stderr, "Vertex shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(vertShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) 
		{
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

	//m_sceneReader = SceneReader("assets/scenes/Scene.xml");

	//for (int i = 0; i < m_sceneReader.m_modelList.size(); i++)
	//{
	//	if (!m_sceneReader.m_modelList.at(i).getCollected()) // Draw all items except collected collectables
	//	{
	//		m_sceneReader.m_modelList[i].initModel();
	//	}
	//	/*Editor.ModelList[i].DrawModel(true, true);*/
	//}
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Defines angular offset of mouse position from the window center
	sf::Vector2f mouseMovement((((float)m_windowSize.x * 0.5f) - (float)m_mousePos.x) / 1000.0f, (((float)m_windowSize.y * 0.5f) - (float)m_mousePos.y) / 1000.0f);

	// Updates the camera with user input
	m_camera.processUserInput(mouseMovement.x, mouseMovement.y);

	// Defines the View
	glm::mat4 V = glm::lookAt( m_camera.getPosition(), m_camera.getView(), m_camera.getUp()); // Position, direction, up

	// Defines the Perspective
	glm::mat4 P = glm::perspective(m_camera.getFOV(), (float)m_windowSize.x / (float)m_windowSize.y, 1.f, 5000.f); // FOV, display aspect ratio and vision culls

	// Passes View and Perspective data to shader
	GLuint viewMatrixID = gl::GetUniformLocation(m_programHandle, "mView");
	GLuint projectionMatrixID = gl::GetUniformLocation(m_programHandle, "mProjection");
	gl::UniformMatrix4fv(viewMatrixID, 1, gl::FALSE_, glm::value_ptr(V));
	gl::UniformMatrix4fv(projectionMatrixID, 1, gl::FALSE_, glm::value_ptr(P));
		
	// Resets cursor to the center of the window after cursor event
	glfwSetCursorPos(m_pWindow, getWindowSize().x*0.5, getWindowSize().y*0.5);

	/////////////////// MODEL PROCESSING ///////////////////
	// Need a Model to place
	// Scrolling cycles through available Models?

	// Model at position infront of Camera with a Y of default: 0

	// LEFT/RIGHT rotate Model?
	// UP/DOWN translate Model in y axis?

	// Left click locks Model into position
	// Model is added to vector of placed Models? Later saved to XML
}

// Void: Renders the Editor to display
void Editor::render()
{
	// Check depth and clear last frame
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
	
	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		pModel->buffer();
		
		// Passes Model transformation data to shader
		GLuint modelMatrixID = gl::GetUniformLocation(m_programHandle, "mModel");
		gl::UniformMatrix4fv(modelMatrixID, 1, gl::FALSE_, glm::value_ptr(pModel->m_M));

		// Renders Model
		pModel->render();
	}
}
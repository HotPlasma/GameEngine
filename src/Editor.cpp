
#include "Editor.h"

// Constructor
Editor::Editor(const sf::Vector2i kWindowSize)
{
	// Assigns input to appropriate member
	m_windowSize = kWindowSize;

	// TEMPORARY - Need to read in a list of models to use
	// Creates a tree Model
	std::shared_ptr<Model> model = std::shared_ptr<Model>(new Model());
	model->setName("Tree");
	model->setFileLocation("assets/models/deadtree.obj");
	model->setTextureLocation("assets/textures/bark.bmp");
	model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	model->setMaterial(1);
	// Populates the ModelSelection member with Models
	m_pModelSelection.push_back(model);

	// Creates a battery Model
	model = std::shared_ptr<Model>(new Model());
	model->setName("Stump");
	model->setFileLocation("assets/models/stump.obj");
	model->setTextureLocation("assets/textures/stump.bmp");
	model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	model->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	model->setMaterial(1);
	// Populates the ModelSelection member with Models
	m_pModelSelection.push_back(model);

	// For every Model
	for (std::shared_ptr<Model> pModel : m_pModelSelection)
	{
		// Loads Model so it's ready for drawing
		pModel->loadModel();

		// Initialises Models
		pModel->initModel();
	}

	// Sets the first Model in the selection to selected
	m_pSelectedModel = m_pModelSelection.front();
}

// Void: Links vert and frag shaders into a glslprogram
void Editor::linkMe(const GLint kVertShader, const GLint kFragShader)
{
	try
	{
		m_shader.compileShader("Shaders/shader.vert");
		m_shader.compileShader("Shaders/shader.frag");
		m_shader.link();
		m_shader.validate();
		m_shader.use();
	}
	catch (GLSLProgramException & e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

// Void: Initialises the Editor Scene
void Editor::initScene(GLFWwindow* pWindow)
{
	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	// Window pointer added to member
	m_pWindow = pWindow;

	// Sets the cursor to be hidden
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Links shaders
	linkMe(1, 2);
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
	m_shader.setUniform("mView", V);
	m_shader.setUniform("mProjection", P);
	
	// Resets cursor to the center of the window after cursor event
	glfwSetCursorPos(m_pWindow, getWindowSize().x*0.5, getWindowSize().y*0.5);

	/////////////////// MODEL PROCESSING ///////////////////
	// Need a Model to place
	// Scrolling cycles through available Models?

	// Hand position infront of Camera with a Y of default: 0
	m_handPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	// Sets Model position to hand plus the selected height
	m_pSelectedModel->setPosition(m_handPosition + glm::vec3(0.0f, m_fSelectionY, 0.0f));

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
	
	// Renders the Model in hand
	m_pSelectedModel->buffer();

	// Passes Model transformation data to shader
	m_shader.setUniform("mModel", m_pSelectedModel->m_M);

	// Renders Model
	m_pSelectedModel->render();

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		pModel->buffer();
		
		// Passes Model transformation data to shader
		m_shader.setUniform("mModel", pModel->m_M);
	
		// Renders Model
		pModel->render();
	}
}
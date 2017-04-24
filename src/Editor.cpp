
#include "Editor.h"

#define MOVE_VELOCITY 50.0f
#define ROTATE_VELOCITY 0.0025f

// Constructor
Editor::Editor(GLFWwindow *pWindow, sf::Vector2i windowSize)
{
	// Sets members with input
	m_pWindow = pWindow;
	m_windowSize = windowSize;

	// Updates Camera aspect ratio
	m_camera.setAspectRatio((float)windowSize.x / windowSize.y);

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
void Editor::linkShaders()
{
	try
	{
		// Shader which allows first person camera and textured objects
		m_worldShader.compileShader("Shaders/shader.vert");
		m_worldShader.compileShader("Shaders/shader.frag");
		m_worldShader.link();
		m_worldShader.validate();
		m_worldShader.use();
	}
	catch (GLSLProgramException & e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		// Shader which allows heads up display
		m_freeType.compileShader("Shaders/freetype.vert");
		m_freeType.compileShader("Shaders/freetype.frag");
		m_freeType.link();
		m_freeType.validate();
		m_freeType.use();
	}
	catch (GLSLProgramException & e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

	try
	{
		// Shader which allows heads up display
		m_imageType.compileShader("Shaders/image.vert");
		m_imageType.compileShader("Shaders/image.frag");
		m_imageType.link();
		m_imageType.validate();
	}
	catch (GLSLProgramException & e)
	{
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

// Void: Initialises the Editor Scene
void Editor::initScene(Freetype* pOverlay)
{
	m_pHUD = pOverlay; // Get the Heads up display for the scene

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	linkShaders();
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));

	m_camera.rotate(delta.x*ROTATE_VELOCITY, delta.y*ROTATE_VELOCITY);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, -MOVE_VELOCITY*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_camera.move(glm::vec3(-MOVE_VELOCITY*kfTimeElapsed, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, MOVE_VELOCITY*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_camera.move(glm::vec3(MOVE_VELOCITY*kfTimeElapsed, 0.0f, 0.0f));
	}

	/////////////////// MODEL PROCESSING ///////////////////
	// Need a Model to place
	// Scrolling cycles through available Models?

	// Hand position infront of Camera with a Y of default: 0
	m_handPosition = glm::vec3(m_camera.getView()[3][0], m_camera.getView()[3][1], m_camera.getView()[3][2]);

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

	// Activates use of shader
	m_worldShader.use();

	glm::mat4 model = glm::mat4(1.0);
	mat4 mv = m_camera.getView() * model;
	m_worldShader.setUniform("ModelViewMatrix", mv);
	m_worldShader.setUniform("NormalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
	m_worldShader.setUniform("MVP", m_camera.getProjection() * mv);
	mat3 normMat = glm::transpose(glm::inverse(mat3(model)));
	m_worldShader.setUniform("M", model);
	m_worldShader.setUniform("V", m_camera.getView());
	m_worldShader.setUniform("P", m_camera.getProjection());

	// Renders the Model in hand
	m_pSelectedModel->buffer();

	// Passes Model transformation data to shader
	m_worldShader.setUniform("M", m_pSelectedModel->m_M);

	// Renders Model
	m_pSelectedModel->render();

	// Render all Models in the Scene
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Buffers the Model
		pModel->buffer();
		
		// Passes Model transformation data to shader
		m_worldShader.setUniform("M", pModel->m_M);
	
		// Renders Model
		pModel->render();
	}

	m_freeType.use();
	m_freeType.setUniform("projection", glm::ortho(0.0f, 1920.0f, 0.f, 1080.f));
	m_pHUD->RenderText(m_freeType.getHandle(), "Collectable Collected", 100.f, 100.f, 1.0f, glm::vec3(0.3, 0.7f, 0.9f));
}
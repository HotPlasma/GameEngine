
#include "Editor.h"

#define HAND_SPEED 25.0f

#define CAMERA_ROTATION 0.0025f
#define CAMERA_SPEED 50.0f

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

	// Creates a stump Model
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
		// Shader which allows first person camera and textured object rendering
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
		// Shader which allows heads up display rendering
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
		// Shader which allows for image rendering
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
	// Get the Heads up display for the scene
	m_pHUD = pOverlay;

	// Enables OpenGL depth testing
	gl::Enable(gl::DEPTH_TEST);

	linkShaders();
}

void Editor::keyPress(const int kiKey)
{
	// Left click locks Model into position
	// Model is added to vector of placed Models? Later saved to XML
	if (kiKey == GLFW_KEY_SPACE)
	{
		m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_pSelectedModel.get())));
		if (!m_pModels.empty())
		{
			m_pModels.back()->setRotation(glm::vec3(0.5f, 0.5f, 0.5f));
		}
	}
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));

	m_camera.rotate(delta.x*CAMERA_ROTATION, delta.y*CAMERA_ROTATION);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		m_camera.move(glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		m_camera.move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		m_camera.move(glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	//{
	//	m_camera.move(glm::vec3(0.0f, -CAMERA_SPEED*kfTimeElapsed, 0.0f));
	//}
	//
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl))
	//{
	//	m_camera.move(glm::vec3(0.0f, CAMERA_SPEED*kfTimeElapsed, 0.0f));
	//}

	/////////////////// MODEL PROCESSING ///////////////////
	// Need a Model to place
	// Scrolling cycles through available Models?
	
	// direction unit vector * distance to plane
	// distance to plane is the 10,0 to 0,10
	// two triangles with trig and shit
	//
	//glm::vec2 hypotenuse;
	//
	//
	//float distance = Utils::magnitude(m_camera.getView() - m_camera.getPosition());
	//
	// Ray out of camera towards where cam is looking?
	//glm::vec4 result = glm::vec4(m_camera.getPosition(), 1.0) * P;
	//
	// Unit vector of the view direction
	//glm::vec3 viewUnitVec = 
	//
	//std::cout 
	//	<< "\nm_camera.getView()[3][0] " << m_camera.getView()[3][0]
	//	<< "\nm_camera.getView()[3][1] " << m_camera.getView()[3][1]
	//	<< "\nm_camera.getView()[3][2] " << m_camera.getView()[3][2]
	//	<< "\n"						   
	//	<< "\nm_camera.getView()[2][0] " << m_camera.getView()[2][0]
	//	<< "\nm_camera.getView()[2][1] " << m_camera.getView()[2][1]
	//	<< "\nm_camera.getView()[2][2] " << m_camera.getView()[2][2]
	//	<< "\n"						   
	//	<< "\nm_camera.getView()[1][0] " << m_camera.getView()[1][0]
	//	<< "\nm_camera.getView()[1][1] " << m_camera.getView()[1][1]
	//	<< "\nm_camera.getView()[1][2] " << m_camera.getView()[1][2]
	//	<< "\n"						   
	//	<< "\nm_camera.getView()[0][0] " << m_camera.getView()[0][0]
	//	<< "\nm_camera.getView()[0][1] " << m_camera.getView()[0][1]
	//	<< "\nm_camera.getView()[0][2] " << m_camera.getView()[0][2]
	//<< std::endl;
	//
	//std::cout
	//	<< "\nm_camera.getView()[3][0] " << m_camera.getView()[3][0]
	//	<< "\nm_camera.getView()[3][1] " << m_camera.getView()[3][1]
	//	<< "\nm_camera.getView()[3][2] " << m_camera.getView()[3][2]
	//	<< "\n"
	//	<< "\nm_camera.getPosition().x " << m_camera.getPosition().x
	//	<< "\nm_camera.getPosition().y " << m_camera.getPosition().y
	//	<< "\nm_camera.getPosition().z " << m_camera.getPosition().z
	//	<< "\n"
	//	<< "\nm_camera.getView()[2][0] - m_camera.getPosition().x " << m_camera.getView()[2][0] - m_camera.getPosition().x
	//	<< "\nm_camera.getView()[2][1] - m_camera.getPosition().y " << m_camera.getView()[2][1] - m_camera.getPosition().y
	//	<< "\nm_camera.getView()[2][2] - m_camera.getPosition().z " << m_camera.getView()[2][2] - m_camera.getPosition().z
	//	<< "\n"
	//	<< "\nglm::degrees(m_camera.getView()[2][0] - m_camera.getPosition().x) " << glm::degrees(m_camera.getView()[2][0] - m_camera.getPosition().x)
	//	<< "\nglm::degrees(m_camera.getView()[2][1] - m_camera.getPosition().y) " << glm::degrees(m_camera.getView()[2][1] - m_camera.getPosition().y)
	//	<< "\nglm::degrees(m_camera.getView()[2][2] - m_camera.getPosition().z) " << glm::degrees(m_camera.getView()[2][2] - m_camera.getPosition().z)
	//<< std::endl;
	//
	//std::cout
	//	<< "\nm_camera.getDirection().x " << m_camera.getDirection().x
	//	<< "\nm_camera.getDirection().y " << m_camera.getDirection().y
	//	<< "\nm_camera.getDirection().z " << m_camera.getDirection().z
	//	<< "\n"																							  
	//<< std::endl;
	//
	//glm::vec3 camView(m_camera.getDirection());
	//
	//float Theta = acos(glm::dot(camView, glm::vec3(0.0f, -1.0f, 0.0f)));
	//float ThetaDegrees = glm::degrees(Theta);
	//
	////float Theta = viewUnitVec.y;
	//float Phi = acos(glm::dot(camView, glm::vec3(0.0f, -1.0f, 0.0f)));
	//
	//float B = m_camera.getPosition().y * -tanf(Theta);
	////float A = B * cosf(Theta);
	////float C = B * sinf(Theta);
	////float L = m_camera.getPosition().y / cosf(Theta);
	//float L = m_camera.getPosition().y / cosf(Theta);
	//
	//// Hand position 
	//m_handPosition = camView * L;
	////m_handPosition = glm::vec3(A, 0.0, B);
	//
	// Hand position infront of Camera with a Y of default: 0
	//m_handPosition = glm::vec3(m_camera.getView()[3][0], m_camera.getView()[3][1], m_camera.getView()[3][2]);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_handPosition += glm::vec3(0.0f, 0.0f, -HAND_SPEED*kfTimeElapsed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_handPosition += glm::vec3(-HAND_SPEED*kfTimeElapsed, 0.0f, 0.0f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_handPosition += glm::vec3(0.0f, 0.0f, HAND_SPEED*kfTimeElapsed);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_handPosition += glm::vec3(HAND_SPEED*kfTimeElapsed, 0.0f, 0.0f);
	}

	// Sets Model position to hand plus the selected height
	m_pSelectedModel->setPosition(m_handPosition + glm::vec3(0.0f, m_fSelectionY, 0.0f));
	//m_pSelectedModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	// LEFT/RIGHT rotate Model?
	// UP/DOWN translate Model in y axis?
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
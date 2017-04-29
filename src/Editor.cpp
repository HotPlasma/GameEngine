
#include "Editor.h"

#define HAND_ROTATION 180.0f
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

// Void: Called on keyPress event
void Editor::keyPress(const int kiKey)
{
	// If Enter key is pressed
	if (kiKey == GLFW_KEY_ENTER)
	{
		// Model is added to a vector of placed Models
		m_pModels.push_back(std::shared_ptr<Model>(new Model(*m_pSelectedModel.get())));
	}

	// If F5 key is pressed
	if (kiKey == GLFW_KEY_F5)
	{
		// Saves Scene to file
		save();
	}
}

// Void: Updates the Editor with elapsed time
void Editor::update(const float kfTimeElapsed)
{
	/////////////////// USER DISPLAY PROCESSING ///////////////////
	// Calculates the mouse movement
	sf::Vector2f delta(m_mousePos - sf::Vector2f(m_windowSize.x * 0.5f, m_windowSize.y * 0.5f));
	// Applies movement of mouse to Camera rotation
	m_camera.rotate(delta.x*CAMERA_ROTATION, delta.y*CAMERA_ROTATION);

	// If W key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		// Move in the Z axis 
		m_camera.move(glm::vec3(0.0f, 0.0f, -CAMERA_SPEED*kfTimeElapsed));
	}
	// If S key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
	{
		// Move in the Z axis 
		m_camera.move(glm::vec3(0.0f, 0.0f, CAMERA_SPEED*kfTimeElapsed));
	}

	// If A key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		// Move in the X axis 
		m_camera.move(glm::vec3(-CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}
	// If D key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		// Move in the X axis 
		m_camera.move(glm::vec3(CAMERA_SPEED*kfTimeElapsed, 0.0f, 0.0f));
	}

	// If Spacebar is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		// Move in the Y axis 
		m_camera.move(glm::vec3(0.0f, -CAMERA_SPEED*kfTimeElapsed, 0.0f));
	}
	// If LShift is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
	{
		// Move in the Y axis 
		m_camera.move(glm::vec3(0.0f, CAMERA_SPEED*kfTimeElapsed, 0.0f));
	}

	/////////////////// MODEL PROCESSING ///////////////////
	
	// Scrolling cycles through available Models?
	//
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

	// TEMPORARY KEYBOARD CONTROLS
	// If Up arrow key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		m_handPosition += m_camera.getZAxis() * glm::vec3(0.0f, 0.0f, -HAND_SPEED*kfTimeElapsed);
	}
	// If Down arrow key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		m_handPosition += m_camera.getZAxis() * glm::vec3(0.0f, 0.0f, HAND_SPEED*kfTimeElapsed);
	}
	// If Left arrow key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		m_handPosition += m_camera.getXAxis() * glm::vec3(-HAND_SPEED*kfTimeElapsed, 0.0f, 0.0f);
	}
	// If Right arrow key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		m_handPosition += m_camera.getXAxis() * glm::vec3(HAND_SPEED*kfTimeElapsed, 0.0f, 0.0f);
	}
	// If PageUp key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageUp))
	{
		m_handPosition += m_camera.getYAxis() * glm::vec3(0.0f, HAND_SPEED*kfTimeElapsed, 0.0f);
	}
	// If PageDown key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::PageDown))
	{
		m_handPosition += m_camera.getYAxis() * glm::vec3(0.0f, -HAND_SPEED*kfTimeElapsed, 0.0f);
	}

	// If Left mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_handRotation += glm::vec3(0.0f, -HAND_ROTATION*kfTimeElapsed, 0.0f);
	}
	// If Right mouse button is pressed
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		m_handRotation += glm::vec3(0.0f, HAND_ROTATION*kfTimeElapsed, 0.0f);
	}

	//// Sets Model position to hand position
	m_pSelectedModel->setPosition(m_handPosition);
	//// Sets Model rotation to hand position
	m_pSelectedModel->setRotation(m_handRotation);

	////m_pSelectedModel->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//
	//// LEFT/RIGHT rotate Model?
	//// UP/DOWN translate Model in y axis?
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
	m_freeType.setUniform("projection", glm::ortho(0.0f, float(m_windowSize.x), 0.f, float(m_windowSize.y)));
	std::string sText; sText += "Model Position: x("; sText += std::to_string(m_handPosition.x); sText += ") y("; sText += std::to_string(m_handPosition.y); sText += ") z("; sText += std::to_string(m_handPosition.z); sText += ")";
	m_pHUD->RenderText(m_freeType.getHandle(), sText, 100.f, 100.f, 1.0f, glm::vec3(0.7, 0.3f, 0.3f));
}

// Void: Saves the Scene to XML file
void Editor::save()
{
	std::cerr << "[EDITOR] Saving to file..." << std::endl;

	// Defines new XML Document
	tinyxml2::XMLDocument document;

	// Creates an XML declaration
	//tinyxml2::XMLDeclaration* declaration = new tinyxml2::XMLDeclaration("1.0", "", "");

	// Creates <Scene> within the document
	tinyxml2::XMLNode* pScene = document.NewElement("Scene");
	document.InsertFirstChild(pScene);

	// For all Models in the Editor
	for (std::shared_ptr<Model> pModel : m_pModels)
	{
		// Defines new element for <Object>
		tinyxml2::XMLNode* pObject = document.NewElement("Object");
		pScene->InsertEndChild(pObject);

		// Defines new element for <Name>
		tinyxml2::XMLElement* pName = document.NewElement("Name");
		// Sets <Name> value to the Model name
		pName->SetText(pModel->getName().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pName);

		// Defines new element for <OBJLocation>
		tinyxml2::XMLElement* pObjLoc = document.NewElement("OBJLocation");
		// Sets <OBJLocation> value to the Model file location
		pObjLoc->SetText(pModel->getFileLocation().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pObjLoc);

		// Defines new element for <TexLocation>
		tinyxml2::XMLElement* pTexLoc = document.NewElement("TexLocation");
		// Sets <TexLocation> value to the Model texture location
		pTexLoc->SetText(pModel->getTexFileLocation().c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTexLoc);

		// Defines new element for <Translation>
		tinyxml2::XMLElement* pTrans = document.NewElement("Translation");
		// Defines string for Translation data
		std::string sTData = std::to_string(pModel->getPosition().x); sTData += " "; sTData += std::to_string(pModel->getPosition().y); sTData += " "; sTData += std::to_string(pModel->getPosition().z);
		// Sets <Translation> value to the Model position
		pTrans->SetText(sTData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pTrans);

		// Defines new element for <Rotation>
		tinyxml2::XMLElement* pRot = document.NewElement("Rotation");
		// Defines string for Rotation data
		std::string sRData = std::to_string(pModel->getRotation().x); sRData += " "; sRData += std::to_string(pModel->getRotation().y); sRData += " "; sRData += std::to_string(pModel->getRotation().z);
		// Sets <Rotation> value to Model rotation
		pRot->SetText(sRData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pRot);

		// Defines new element for <Scale>
		tinyxml2::XMLElement* pScale = document.NewElement("Scale");
		// Defines string for Scale data
		std::string sSData = std::to_string(pModel->getScale().x); sSData += " "; sSData += std::to_string(pModel->getScale().y); sSData += " "; sSData += std::to_string(pModel->getScale().z);
		// Sets <Scale> value to the Model scale
		pScale->SetText(sSData.c_str());
		// Inserts element into <Object>
		pObject->InsertEndChild(pScale);

		// Defines new element for <Material>
		tinyxml2::XMLElement* pMaterial = document.NewElement("Material");
		// Sets <Material> value to Model scale
		pMaterial->SetText(pModel->getMaterial());
		// Inserts element into <Object>
		pObject->InsertEndChild(pMaterial);

		// Defines new element for <Collectable>
		tinyxml2::XMLElement* pCollectable = document.NewElement("Collectable");
		// Sets <Collectable> value to Model name
		pCollectable->SetText(int(pModel->isCollectable()));
		// Inserts element into <Object>
		pObject->InsertEndChild(pCollectable);
	}

	// Saves the document
	document.SaveFile(m_sFilepath.c_str());

	std::cerr << "[EDITOR] File saved." << std::endl;
}
#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "ModelReader.h"
#include "glslprogram.h"

//!< Scene subclass for creating levels
class Editor : public Scene
{
	private:
		
		std::string m_sFilepath = "assets/scenes/editorScene.xml"; //!< Scene file path

		std::vector<std::shared_ptr<Model>> m_pModels; //!< Scene Model object ptrs

		glm::vec3 m_handPosition; //!< User's 'hand' position
		glm::vec3 m_handRotation; //!< User's 'hand' rotation

		//float m_fSelectionY; //!< Position of selection in the Y axis

		std::vector<std::shared_ptr<Model>> m_pModelSelection; //!< Models available to be placed
		std::shared_ptr<Model> m_pSelectedModel; //!< Model in the user's 'hand'

		void linkShaders(); //!< Links vert and frag shaders into a glslprogram
	
	public:
	
		Editor(GLFWwindow *pWindow, sf::Vector2i windowSize); //!< Constructor

		void initScene(Freetype* pOverlay); //!< Initialises the Editor Scene

		void save(); //!< Saves the Scene to XML file

		void keyPress(const int kiKey); //!< Called on keyPress event
		void mouseScroll(const double kdDelta); //!< Called on mouseScroll event

		void update(const float kfTimeElapsed); //!< Updates the Editor with elapsed time
		void render(); //!< Renders the Editor to display

		void setMousePos(const sf::Vector2f kMousePos) { m_mousePos = kMousePos; } //!< Sets Mouse position
};
#endif  

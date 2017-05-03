#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "ModelReader.h"
#include "glslprogram.h"
#include "Button.h"

struct Selection
{
	// Object properties
	bool m_bCollectable = false;
	bool m_bAI = false;

	// Transformation members
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f); //!< Selection position
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f); //!< Selection rotation
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f); //!< Selection scale

	unsigned int m_uiMaterial = 1; //!< Model material preset

	std::shared_ptr<Model> m_pModel; //!< Selection Model
};

struct EditorHUD
{
	// Buttons for Object properties
	std::shared_ptr<Button> m_pCollectable; //!< Collectable toggle
	std::shared_ptr<Button> m_pAI; //!< AI toggle

	// Buttons for transformation mode switching
	std::shared_ptr<Button> m_pTranslateMode; //!< Translate mode button
	std::shared_ptr<Button> m_pRotateMode; //!< Rotate mode button
	std::shared_ptr<Button> m_pScaleMode; //!< Scale mode button

	std::shared_ptr<Button> m_pSave; //!< Button to save Scene to file
};

//!< Scene subclass for creating levels
class Editor : public Scene
{
	private:
		
		std::string m_sFilepath = "assets/scenes/editorScene.xml"; //!< Scene file path TEMPORARY default

		std::vector<std::shared_ptr<Model>> m_pModels; //!< Scene Model object ptrs

		Selection m_selection; //!< User's 'hand'

		sf::Vector2f m_lastMousePos; //!< Last cursor position

		enum TransformationMode { TRANSLATE, ROTATE, SCALE }; //!< Enum for which transformation mode the Editor is in
		TransformationMode m_transformMode; //!< Editor's current transformation mode

		EditorHUD m_buttons; //!< Editor option buttons

	public:
	
		Editor(GLFWwindow *pWindow, const sf::Vector2i kWindowSize); //!< Constructor

		void initScene(Freetype* pOverlay); //!< Initialises the Editor Scene

		void save(); //!< Saves the Scene to XML file

		void input_key(const int kiKey, const int kiAction); //!< Called on key input event
		void input_button(const int kiButton, const int kiAction); //!< Called on mouseButton input event
		void input_scroll(const double kdDelta); //!< Called on mouseScroll input event

		void update(const float kfTimeElapsed); //!< Updates the Editor with elapsed time
		void render(); //!< Renders the Editor to display
};
#endif  

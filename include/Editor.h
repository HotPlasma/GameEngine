#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "ModelReader.h"
#include "Button.h"
#include "TextBox.h"
#include "Utilities.h"

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

	std::vector<std::shared_ptr<Model>> m_pRedoList; //!< Models that have been undone

	bool m_flashGoingUp = true; // Whether flash are moving up
	float m_flashOffset = 0.0f; // Light intensity for selected Model
	Bounds m_flashBounds = Bounds(0.15f, -0.15f); // Upper and lower bounds for flash intensity
};

struct EditorHUD
{
	// Buttons for Model settings
	std::shared_ptr<Button> m_pModel; //!< Model selection
	std::shared_ptr<Button> m_pUndo; //!< Undo button
	std::shared_ptr<Button> m_pRedo; //!< Redo button

	// Buttons for Object properties
	std::shared_ptr<ToggleButton> m_pCollectable; //!< Collectable toggle
	std::shared_ptr<ToggleButton> m_pAI; //!< AI toggle

	// Buttons for transformation mode switching
	std::shared_ptr<ToggleButton> m_pTranslateMode; //!< Translate mode button
	std::shared_ptr<ToggleButton> m_pRotateMode; //!< Rotate mode button
	std::shared_ptr<ToggleButton> m_pScaleMode; //!< Scale mode button

	std::shared_ptr<Button> m_pSave; //!< Button to save Scene to file
};

struct ModelMenu
{
	unsigned int m_uiBGIndex; //!< Background imageplane index

	std::shared_ptr<TextBox> m_pActiveField; //!< Text box active for writing

	std::shared_ptr<TextBox> m_pNameField; //!< Text box for Model name
	std::shared_ptr<TextBox> m_pObjField; //!< Text box for Model obj file location
	std::shared_ptr<TextBox> m_pTexField; //!< Text box for Model texture file location

	std::shared_ptr<Button> m_pLoad; //!< Load Model button
	std::shared_ptr<Button> m_pCancel; //!< Cancel button

	void reset() //!< Resets field data and active field
	{
		// Clears menu fields
		m_pNameField->setStr("");
		m_pObjField->setStr("");
		m_pTexField->setStr("");

		// Resets active field to name
		m_pActiveField = m_pNameField;
	}
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

	bool m_bMenuOpen = false; //!< Whether Model menu is active
	ModelMenu m_menu; //!< Model selection menu

	std::shared_ptr<Model> m_pSkybox; //!< World skybox Model

	void save(); //!< Saves the Scene to XML file

public:

	Editor(GLFWwindow *pWindow, const sf::Vector2i kWindowSize); //!< Constructor

	void initScene(Freetype* pOverlay); //!< Initialises the Editor Scene

	void input_key(const int kiKey, const int kiAction); //!< Called on key input event
	void input_char(const unsigned int kuiUnicode); //!< Called on char input event
	void input_button(const int kiButton, const int kiAction); //!< Called on mouseButton input event
	void input_scroll(const double kdDelta); //!< Called on mouseScroll input event

	void update(const float kfTimeElapsed); //!< Updates the Editor with elapsed time
	void render(); //!< Renders the Editor to display
};

#endif
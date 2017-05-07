#pragma once
#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "SceneReader.h"
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

	Bouncer m_lightIntensity = Bouncer(Bounds(0.15f, -0.15f)); //!< Bouncing value for selection light intensity
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
	std::shared_ptr<Button> m_pLoad; //!< Button to load Scene from file
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

struct FileMenu
{
	unsigned int m_uiBGIndex; //!< Background imageplane index

	std::shared_ptr<TextBox> m_pFileField; //!< Text box for file path

	std::shared_ptr<Button> m_pEnter; //!< Enter filepath button
	std::shared_ptr<Button> m_pCancel; //!< Cancel button

	void reset() //!< Resets field data and active field
	{
		// Clears menu fields
		m_pFileField->setStr("");
	}
};

//!< Scene subclass for creating levels
class Editor : public Scene
{
private:
	
	float m_fAutosaveTimer = 0.0f; //!< Counts up in seconds - triggers autosaving
	
	std::vector<std::shared_ptr<Model>> m_pModels; //!< Scene Model object ptrs

	Selection m_selection; //!< User's 'hand'

	sf::Vector2f m_lastMousePos; //!< Last cursor position

	enum TransformationMode { TRANSLATE, ROTATE, SCALE }; //!< Enum for which transformation mode the Editor is in
	TransformationMode m_transformMode = TRANSLATE; //!< Editor's current transformation mode

	Bouncer m_collFlash = Bouncer(Bounds(0.4f, 0.0f)); //!< Bouncing value for collectable light intensity
	Bouncer m_aiFlash = Bouncer(Bounds(0.4f, 0.0f)); //!< Bouncing value for collectable light intensity

	std::shared_ptr<Model> m_pSpawn; //!< Spawn indicator Model
	std::shared_ptr<Model> m_pSkybox; //!< World skybox Model

	PopUp m_popUp; //!< PopUp for HUD display messages

	EditorHUD m_buttons; //!< Editor option buttons

	enum EditorState { EDITOR, MENU_SELECT, MENU_SAVE, MENU_LOAD }; //!< Enum for which state the Editor is in
	EditorState m_state = EDITOR; //!< Editor's current state

	ModelMenu m_select; //!< Model selection menu
	FileMenu m_save; //!< File save menu
	FileMenu m_load; //!< File load menu
	
	void button_select(); //!< ModelSelection button action
	void button_save(); //!< SaveScene button action
	void button_load(); //!< LoadScene button action

	void save(const std::string ksFilePath); //!< Saves the Scene to XML file
	void load(const std::string ksFilePath); //!< Loads a Scene into the Editor

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
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

		GLSLProgram m_shader; //!< Shader program
		glm::mat4 m_M; //!< Model transformation matrix
		Camera m_camera; //!< User Camera

		GLFWwindow* m_pWindow; //!< Display Window ptr
		sf::Vector2i m_mousePos; //!< Mouse cursor position
	
		std::vector<std::shared_ptr<Model>> m_pModels; //!< Scene Model object ptrs
		Texture* m_pTexture; //!< Model Texture ptr

		glm::vec3 m_handPosition; //!< User's 'hand' position
		float m_fSelectionY; //!< Position of selection in the Y axis
		std::vector<std::shared_ptr<Model>> m_pModelSelection; //!< Models available to be placed
		std::shared_ptr<Model> m_pSelectedModel; //!< Model in the user's 'hand'

		void linkMe(const GLint kVertShader, const GLint kFragShader); //!< Links vert and frag shaders into a glslprogram
	
	public:
	
		Editor(const sf::Vector2i kWindowSize); //!< Constructor

		void initScene(GLFWwindow* pWindow); //!< Initialises the Editor Scene
		void update(const float kfTimeElapsed); //!< Updates the Editor with elapsed time
		void render(); //!< Renders the Editor to display

		void setMousePos(const sf::Vector2i kMousePosition) { m_mousePos = kMousePosition; } //!< Sets Mouse position
};
#endif  
#ifndef EDITOR_H
#define EDITOR_H

#include "PreHeader.h"

#include "Scene.h"
#include "Camera.h"
#include "SceneReader.h"
#include "ModelReader.h"

//!< Scene subclass for creating levels
class Editor : public Scene
{
	private:
	
		std::vector<std::shared_ptr<Model>> m_pModels; //!< Scene Model object ptrs
		Texture* m_pTexture; //!< Model Texture ptr

		GLuint m_programHandle; //!< Shader handle context
		glm::mat4 m_M; //!< Model transformation matrix
		Camera m_camera; //!< User Camera

		GLFWwindow* m_pWindow; //!< Display Window ptr
		sf::Vector2i m_mousePos; //!< Mouse cursor position
	
		void linkMe(const GLint kVertShader, const GLint kFragShader); //!< Links vert and frag shaders into a glslprogram
	
	public:
	
		Editor(const sf::Vector2i kWindowSize); //!< Constructor

		void initScene(GLFWwindow* pWindow); //!< Initialises the Editor Scene
		void update(const float kfTimeElapsed); //!< Updates the Editor with elapsed time
		void render(); //!< Renders the Editor to display

		void setMousePos(const sf::Vector2i kMousePosition) { m_mousePos = kMousePosition; } //!< Sets Mouse position
};
#endif  

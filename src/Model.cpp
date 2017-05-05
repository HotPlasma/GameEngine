#include "Model.h"

Model::Model()
{
	m_M = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

Model::Model(string sFileLocation, string sTextureLocation, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int iMaterialID)
{
	// Constucts a model with the given variables
	m_sFileName = sFileLocation;
	m_sTexture = sTextureLocation;
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	m_iMaterial = iMaterialID;

	m_M = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

void Model::initModel()
{
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	
	glm::mat4 scaleMatrix = { m_scale.x,0,0,0,
		0,m_scale.y,0,0,
		0,0,m_scale.z,0,
		0,0,0,1 };

	glm::mat4 transMatrix = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		m_position.x,m_position.y,m_position.z,1 };

	m_M = scaleMatrix * rotMatrix * transMatrix;

	m_positionData = m_pModelReader->getVertices();
	m_uvData = m_pModelReader->getTextureCoordinates();

	gl::GenBuffers(2, m_vboHandles);
	GLuint positionBufferHandle = m_vboHandles[0];
	GLuint uvBufferHandle = m_vboHandles[1];

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_positionData.size() * sizeof(float), m_positionData.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_uvData.size() * sizeof(float), m_uvData.data(), gl::STATIC_DRAW);

	// Create and set-up the vertex array object
	gl::GenVertexArrays(1, &m_vaoHandle);
	gl::BindVertexArray(m_vaoHandle);

	gl::EnableVertexAttribArray(0);  // Vertex position
	gl::EnableVertexAttribArray(1);  // Vertex color

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);


	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

	gl::BindVertexArray(m_vaoHandle);

	m_bmp = Bitmap::bitmapFromFile(m_sTexture);
	m_bmp.flipVertically();
	m_pTexture = new Texture(m_bmp);
	//Set texture
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, m_pTexture->object());
	GLint loc = gl::GetUniformLocation(m_programHandle, "tex");

	gl::Uniform1f(loc, 1);
}

void Model::render(GLSLProgram* pShader, const glm::mat4 kModel)
{
	// Binds texture
	gl::BindVertexArray(m_vaoHandle);
	gl::BindTexture(gl::TEXTURE_2D, m_pTexture->object());

	// Defines the rotation component
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	rotMatrix = glm::rotate(rotMatrix, m_rotation.x, glm::vec3(1, 0, 0));
	rotMatrix = glm::rotate(rotMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	rotMatrix = glm::rotate(rotMatrix, m_rotation.z, glm::vec3(0, 0, 1));

	// Defines the scale component
	glm::mat4 scaleMatrix = { m_scale.x,0,0,0,
		0,m_scale.y,0,0,
		0,0,m_scale.z,0,
		0,0,0,1 };

	// Defines the translation component
	glm::mat4 transMatrix = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		m_position.x,m_position.y,m_position.z,1 };

	// Combines components into transformation matrix
	m_M = transMatrix  * scaleMatrix * rotMatrix;
	// Applies input tranformation
	m_M *= kModel;

	// Activates use of Shader
	pShader->use();
	// Sends transformation matrix to shader
	pShader->setUniform("M", m_M);

	// Draws Model
	gl::DrawArrays(gl::TRIANGLES, 0, (GLsizei)m_positionData.size());
}

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

Model::Model(string sFileLocation, string sTextureLocation, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int iMaterialID, bool b3D)
{
	// Constucts a model with the given variables
	m_sFileName = sFileLocation;
	m_sTexture = sTextureLocation;
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
	m_iMaterial = iMaterialID;
	m_b3D = b3D;
	m_M = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

void Model::setCollectable(bool NewSetting)
{
	m_bCollectable = NewSetting;
}

void Model::setCollected(bool bCollected)
{
	m_bCollected = bCollected;
}

void Model::setName(string sNewName)
{
	m_sName = sNewName;
}

void Model::setFileLocation(string sNewLocation)
{
	m_sFileName = sNewLocation;
}

void  Model::setTextureLocation(string sNewLocation)
{
	m_sTexture = sNewLocation;
}

void Model::setVisable(bool Visability)
{
	m_bVisible = Visability;
}

void  Model::setPosition(glm::vec3 newPosition)
{
	m_position = newPosition;
}

void  Model::setRotation(glm::vec3 newRotation)
{
	m_rotation = newRotation;
}

void  Model::setScale(glm::vec3 newScale)
{
	m_scale = newScale;
}

void Model::setTexture(GLuint textureID)
{
	m_textureID = textureID;
}

void Model::setMaterial(int iMaterial)
{
	m_iMaterial = iMaterial;
}

void Model::loadModel()
{
	m_pModelReader = new ModelReader(m_sFileName);
}

void Model::buffer()
{
	gl::BindVertexArray(m_vaoHandle);
	gl::BindTexture(gl::TEXTURE_2D, m_pTexture->object());
	
}

CollisionBox Model::getCollisionBox()
{
	CollisionBox newBox;
	newBox.left = m_position.x - (m_BoundingBox.x / 2);
	newBox.right = m_position.x + (m_BoundingBox.x / 2);
	newBox.bottom = m_position.y - (m_BoundingBox.y / 2);
	newBox.top = m_position.y + (m_BoundingBox.y / 2);
	newBox.front = m_position.z - (m_BoundingBox.z / 2);
	newBox.back = m_position.z + (m_BoundingBox.z / 2);

	newBox.max = m_BoundBoxMax;
	newBox.min = m_BoundBoxMin;

	newBox.boundingBox;
	
	return newBox;
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
	m_normalData = m_pModelReader->getNormals();
	m_uvData = m_pModelReader->getTextureCoordinates();

	gl::GenBuffers(3, m_vboHandles);
	GLuint positionBufferHandle = m_vboHandles[0];
	GLuint vertexBufferHandle = m_vboHandles[1];
	GLuint uvBufferHandle = m_vboHandles[2];

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_positionData.size() * sizeof(float), m_positionData.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, vertexBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_normalData.size() * sizeof(float), m_normalData.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, m_uvData.size() * sizeof(float), m_uvData.data(), gl::STATIC_DRAW);

	// Create and set-up the vertex array object
	gl::GenVertexArrays(1, &m_vaoHandle);
	gl::BindVertexArray(m_vaoHandle);

	if (m_b3D)
	{

		gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
		gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
		gl::EnableVertexAttribArray(0);  // Vertex position

		gl::BindBuffer(gl::ARRAY_BUFFER, vertexBufferHandle);
		gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
		gl::EnableVertexAttribArray(1);  // Vertex normal

		gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
		gl::VertexAttribPointer(2, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

		gl::EnableVertexAttribArray(2);  // Vertex color
	}

	else
	{
		gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
		gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
		gl::EnableVertexAttribArray(0);  // Vertex position

		gl::BindBuffer(gl::ARRAY_BUFFER, vertexBufferHandle);
		gl::VertexAttribPointer(2, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);
		gl::EnableVertexAttribArray(1);  // Vertex normal

		gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
		gl::VertexAttribPointer(1, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

		gl::EnableVertexAttribArray(2);  // Vertex color
	}

	gl::BindVertexArray(m_vaoHandle);

	if (m_sTexture.compare("") != 0) // Allow a model to be loaded without a texture without crashing
	{
		m_bmp = Bitmap::bitmapFromFile(m_sTexture);
		m_bmp.flipVertically();
		m_pTexture = new Texture(m_bmp);
		//Set texture
		gl::ActiveTexture(gl::TEXTURE0);
		gl::BindTexture(gl::TEXTURE_2D, m_pTexture->object());
		GLint loc = gl::GetUniformLocation(m_programHandle, "tex");

		gl::Uniform1f(loc, 1);

	}

	m_BoundBoxMax = glm::vec3(-999999.0f, -999999.0f, -999999.0f);
	m_BoundBoxMin = glm::vec3(999999.0f, 999999.0f, 999999.0f);

	for (int i = 0; i < m_positionData.size(); i+=3)
	{
		if (m_positionData.at(i) > m_BoundBoxMax.x)
		{
			m_BoundBoxMax.x = m_positionData.at(i);
		}

		if (m_positionData.at(i + 1) > m_BoundBoxMax.y)
		{
			m_BoundBoxMax.y = m_positionData.at(i+1);
		}

		if (m_positionData.at(i + 2) > m_BoundBoxMax.z)
		{
			m_BoundBoxMax.z = m_positionData.at(i+2);
		}

		if (m_positionData.at(i) < m_BoundBoxMin.x)
		{
			m_BoundBoxMin.x = m_positionData.at(i);
		}

		if (m_positionData.at(i + 1) < m_BoundBoxMin.y)
		{
			m_BoundBoxMin.y = m_positionData.at(i + 1);
		}

		if (m_positionData.at(i + 2) < m_BoundBoxMin.z)
		{
			m_BoundBoxMin.z = m_positionData.at(i + 2);
		}
	}

	m_BoundingBox = glm::vec3(m_BoundBoxMax - m_BoundBoxMin);

}

//rp3d::ConvexMeshShape Model::getCollisionMesh()
//{
//	return m_MeshShape;
//}

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

void Model::playerCaught(bool playerCaught)
{
	m_bplayerCaught = playerCaught;
}

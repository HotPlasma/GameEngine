#include "PreHeader.h"
#include "Model.h"

Model::Model()
{
	M = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
}

Model::Model(string FileLocation, string TextureLocation, glm::vec3 Position, glm::vec3 Rotation, glm::vec3 Scale, int MaterialID)
{
	// Constucts a model with the given variables
	sFileName = FileLocation;
	sTexture = TextureLocation;
	ModelPosition = Position;
	ModelRotation = Rotation;
	ModelScale = Scale;
	ModelMaterial = MaterialID;

	M = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
}

string Model::GetName()
{
	return sName;
}

string Model::GetFileLocation()
{
	return sFileName;
}

GLuint Model::GetTextureLocation()
{
	return m_textureID;
}

glm::vec3 Model::GetPosition()
{
	return ModelPosition;
}

glm::vec3 Model::GetRotation()
{
	return ModelRotation;
}

glm::vec3 Model::GetScale()
{
	return ModelScale;
}

bool Model::CheckIfCollectable()
{
	return m_bCollectable;
}

bool Model::GetCollected()
{
	return false;
}

int Model::GetMaterial()
{
	return m_bCollected;
}

void Model::SetCollectable()
{
	m_bCollectable = true;
}

void Model::SetCollected(bool Collected)
{
	m_bCollected = Collected;
}

void Model::SetName(string newName)
{
	sName = newName;
}



void Model::SetFileLocation(string NewLocation)
{
	sFileName = NewLocation;
}

void  Model::SetTextureLocation(string NewLocation)
{
	sTexture = NewLocation;
}

void  Model::SetPosition(glm::vec3 NewPosition)
{
	ModelPosition = NewPosition;
}

void  Model::SetRotation(glm::vec3 NewRotation)
{
	ModelRotation = NewRotation;
}

void  Model::SetScale(glm::vec3 NewScale)
{
	ModelScale = NewScale;
}

void Model::SetTexture(GLuint TextureID)
{
	m_textureID = TextureID;
}

void Model::SetMaterial(int Material)
{
	ModelMaterial = Material;
}

void Model::LoadModel(string Model)
{
	m_modelReader = new ModelReader(Model);
}

void Model::Buffer()
{
	gl::BindVertexArray(vaoHandle);
	gl::BindTexture(gl::TEXTURE_2D, gTexture->object());
	
}

void Model::DrawModel(bool drawWithNormals, bool drawWithTexture)
{
	/*M = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };*/


	glm::mat4 rotMatrix = { cos(ModelRotation.x),0,-sin(ModelRotation.x),0,
		0,1,0,0,
		sin(ModelRotation.x),0,cos(ModelRotation.x),0,
		0,0,0,1 };

	glm::mat4 scaleMatrix = { ModelScale.x,0,0,0,
		0,ModelScale.y,0,0,
		0,0,ModelScale.z,0,
		0,0,0,1 };

	glm::mat4 transMatrix = { 1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		ModelPosition.x,ModelPosition.y,ModelPosition.z,1 };

	M = scaleMatrix * transMatrix * rotMatrix;


	positionData = m_modelReader->GetVertices();
	uvData = m_modelReader->GetTextureCoordinates();


	gl::GenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint uvBufferHandle = vboHandles[1];


	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, positionData.size() * sizeof(float), positionData.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, uvData.size() * sizeof(float), uvData.data(), gl::STATIC_DRAW);



	// Create and set-up the vertex array object
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);

	gl::EnableVertexAttribArray(0);  // Vertex position
	gl::EnableVertexAttribArray(1);  // Vertex color

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);


	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufferHandle);
	gl::VertexAttribPointer(1, 2, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

	gl::BindVertexArray(vaoHandle);

	bmp = Bitmap::bitmapFromFile(sTexture);
	bmp.flipVertically();
	gTexture = new Texture(bmp);
	//Set texture
	gl::ActiveTexture(gl::TEXTURE0);
	gl::BindTexture(gl::TEXTURE_2D, gTexture->object());
	GLint loc = gl::GetUniformLocation(programHandle, "tex");

	gl::Uniform1f(loc, 1);


}
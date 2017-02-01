#version 430

// Brings in vertex position and texture coordinates from GPU
layout (location=0) in vec3 vertexPosition;
layout (location=1) in vec2 fragTexCoord;

// Brings in uniform variables for MVP matrices
uniform mat4 mModel;        // Matrix to convert to world coordinate system
uniform mat4 mView;         // Matrix to convert into camera coordinate system
uniform mat4 mProjection;   // Matrix to convert to normalised screen coordinates

// Passes texture coordinate to fragment shader
out vec2 texCoord; 

void main()
{
	texCoord = fragTexCoord;
	
	// Transform from local to world to camera to NDCs
	gl_Position = mProjection * mView * mModel * vec4(vertexPosition, 1.0f);
}

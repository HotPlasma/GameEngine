#version 430

out VS_OUT 
{
	vec2 TextureCoord;
	vec3 N;
	vec3 LightPosition;
	vec3 VertexPosition;
} vs_out;

// Vertex positions, normals and texture coordinates
layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec3 VertexNormal;
layout (location=2) in vec2 TextureCoord;


// Create explorable scene using MVP 
// Matrix to convert to world coordinate system
uniform mat4 M;
// Matrix to convert into camera coordinate system 
uniform mat4 V;
// Matrix to convert to normalised screen coordinates
uniform mat4 P;  

uniform mat3 NormalMatrix;
uniform vec3 lightPosition;

void main()
{
	vs_out.TextureCoord = TextureCoord;
	
	// Variables necessary for Phong shading
	vs_out.N = normalize(NormalMatrix * VertexNormal);

	vs_out.VertexPosition = vec3(V * M * vec4(VertexPosition, 1.0));

	vs_out.LightPosition = vec3(V * M * vec4(lightPosition, 1.0));
	
	// Transform from local to world to camera to NDCs
	gl_Position = P * V * M * vec4(VertexPosition, 1.0);
}
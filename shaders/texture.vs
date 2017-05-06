#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;

out VS_OUT 
{
	vec2 TextureCoords;
} vs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	vs_out.TextureCoords = VertexUV;
	
	gl_Position = P * V * M * vec4(VertexPosition, 1.0f);
}

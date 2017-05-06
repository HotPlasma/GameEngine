#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexUV;

out VS_OUT 
{
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextureCoords;
} vs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    vs_out.FragmentPosition = vec3(M * vec4(VertexPosition, 1.0f)).xyz;
	
	mat3 normalMatrix = transpose(inverse(mat3(M)));
    vs_out.Normal = normalize(normalMatrix * VertexNormal);

	vs_out.TextureCoords = VertexUV;
	
	gl_Position = P * V * M * vec4(VertexPosition, 1.0f);
}

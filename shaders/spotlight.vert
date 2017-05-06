#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 TextureCoord;

out VS_OUT
{
	vec3 VertexPosition;
	vec3 N;
	vec2 TextureCoord;
} vs_out;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
	vs_out.VertexPosition = vec3(V * M * vec4(VertexPosition,1.0));

	mat3 normalMatrix = transpose(inverse(mat3(M)));
	vs_out.N = normalize(normalMatrix * VertexNormal);

	vs_out.TextureCoord = TextureCoord;

	gl_Position = P * V * M * vec4(VertexPosition,1.0);
}
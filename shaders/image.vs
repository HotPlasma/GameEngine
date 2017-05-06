#version 430
layout (location = 0) in vec3 vertexPosition_modelspace;
layout (location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 M;        // Matrix to convert to world 
uniform mat4 P;		 // Matrix performance

void main()
{
    gl_Position = P * mat4(1.0f) * M * vec4(vertexPosition_modelspace.xy, 0.0, 1.0);
	UV = vertexUV;
}
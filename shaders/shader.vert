#version 430

in vec4 vertPosition;

// Vertex positions and texture coordinates

layout (location=0) in vec3 VertexPosition;
layout (location=1) in vec2 fragTexCoord;

out vec2 texCoord;

// Create explorable scene using MVP 


uniform mat4 M;        // Matrix to convert to world 
   // Coordinate system
uniform mat4 V;         // Matrix to convert into 
   // Camera coordinate system
uniform mat4 P;   // Matrix to convert to 
   // Normalised screen 
   // Coordinates

in vec3  vertColour;
out vec3 Colour;

void main()
{
texCoord = fragTexCoord;

//Transform from local to world to camera to NDCs
gl_Position = P * V * M * vertPosition;
}

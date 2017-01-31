#version 430

// Binds texture to model based on texture coordinates.

uniform sampler2D tex;
in vec2 texCoord;
out vec4 FragColour;


void main()
{
	FragColour = texture(tex,texCoord);
}

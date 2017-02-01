#version 430

// Brings in uniform variable for texture
uniform sampler2D tex; 

// Is passed texCoord from the vertex shader
in vec2 texCoord; 

// Passes fragment colour out to GPU
out vec4 fragColour; 

void main()
{
	fragColour = texture(tex, texCoord);
}

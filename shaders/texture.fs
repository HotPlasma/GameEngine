#version 430

in VS_OUT 
{
	vec2 TextureCoords;
} vs_out;

uniform sampler2D TextureMap;

layout( location = 0 ) out vec4 FragColour;

void main()
{
	// Returns 
	FragColour = vec4(texture(TextureMap, vs_out.TextureCoords).rgb, 1.0f);
}
#version 430

in VS_OUT 
{
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TextureCoords;
} vs_out;

uniform struct MaterialInfo
{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
} Material;

uniform struct LightInfo
{
	vec3 Position;
	vec3 Intensity;
} Light;

uniform struct SpotlightInfo
{
	vec3 Direction;
	float CutOff;
	float OuterCutOff;
} Spotlight;

uniform sampler2D TextureMap;

layout( location = 0 ) out vec4 FragColour;

void main()
{
	// Defines fragment colour with TextureMap
	vec3 colour = texture(TextureMap, vs_out.TextureCoords).rgb;

	// Calculate the light vector
	vec3 L = normalize(Light.Position - vs_out.FragmentPosition);
	// Calculates Dist from light to vertex
	float D = length(L); //L
	
	// Calculate Ambient Term
	vec4 ambientTerm = vec4(Light.Intensity * colour, 1.0f);
	
	// Calculate Diffuse Term
	vec4 Idv4 = vec4(Light.Intensity, 1.0f) * max(dot(vs_out.Normal,L), 0.0f); 
	Idv4 = clamp(Idv4, 0.0f, 1.0f);
	// Multiply the Reflectivity by the Diffuse intensity
	vec4 diffuseTerm = vec4(colour, 1.0f) * Idv4;
	
	// Calculate Specular Term
	vec4 specularTerm = vec4(Light.Intensity, 1.0f) * vec4(Material.Ks, 1.0f) * pow(max(dot(normalize(reflect(L, normalize(vs_out.Normal))), normalize(Light.Position)), 0.0f), Material.Shininess);
	
	// Calculate Attenuation
	// att = 1 - pow(distance to light source / furthest distance affected by light), falloff distance)
	float attenuation = 1 - pow(D/50, 2);
	
	// Spotlight calculations
	float spotlightTheta = dot(L, normalize(-Spotlight.Direction));
	float spotlightEpsilon = Spotlight.CutOff - Spotlight.OuterCutOff;
	float spotlightIntensity = clamp((spotlightTheta - Spotlight.OuterCutOff) / spotlightEpsilon, 0.0f, 1.0f);  
	
	// Applies spotlight intensity
	//ambientTerm *= spotlight_Intensity; // No intensity applied to ambient so world outside isn't black
	diffuseTerm *= spotlightIntensity;
	specularTerm *= spotlightIntensity;
	
	// FragColour = Ambient term + Attenuation x (Diffuse term + Specular term)
	FragColour = ambientTerm + attenuation * (diffuseTerm + specularTerm);
}
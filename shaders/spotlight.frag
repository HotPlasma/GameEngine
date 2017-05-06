#version 430

in VS_OUT 
{
	vec3 VertexPosition;
	vec3 N;				
	vec2 TextureCoord;
} vs_out;

struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

struct Light 
{
	vec3 position;

    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform Material material;
uniform Light light;

layout( location = 0 ) out vec4 FragColour;

void main() 
{
	vec3 diffuseTexture = texture(material.diffuse, vs_out.TextureCoord).rgb;
	vec3 specularTexture = texture(material.specular, vs_out.TextureCoord).rgb;

	// Calculate the light vector
	vec3 L = normalize(light.position - vs_out.VertexPosition);
	// Calculates Dist from light to vertex
	float D = length(L);
	
	// Calculate Ambient Term
	vec4 ambientTerm = vec4(light.ambient * diffuseTexture, 1.0);
	//vec4 ambientTerm = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	
	// Do lighting calculations
	// Calculate Diffuse Term
	vec4 Idv4 = vec4(light.diffuse, 1.0) * max(dot(vs_out.N,L), 0.0); 
	Idv4 = clamp(Idv4, 0.0, 1.0);
	// Multiply the Reflectivity by the Diffuse intensity
	vec4 diffuseTerm = vec4(diffuseTexture, 1.0f) * Idv4;
	//vec4 diffuseTerm = vec4(0.5f, 0.5f, 0.5f, 1.0f) * Idv4;
	
	// Calculate Specular Term
	vec4 specularTerm = vec4(light.specular, 1.0f) * vec4(0.5f, 0.5f, 0.5f, 1.0f) * pow(max(dot(normalize(reflect(L, normalize(vs_out.N))), normalize(light.position)), 0.0), material.shininess);
	
	// Calculate Attenuation
	// att = 1 - pow(distance to light source / furthest distance affected by light), falloff distance)
	float attenuation = 1 - pow(D/50, 2);
	
	// Spotlight calculations
	float spotlight_Theta = dot(L, normalize(-light.direction));
	float spotlight_Epsilon = light.cutOff - light.outerCutOff;
	float spotlight_Intensity = clamp((spotlight_Theta - light.outerCutOff) / spotlight_Epsilon, 0.0, 1.0);  
	
	// Applies spotlight intensity
	//ambientTerm *= spotlight_Intensity; // No intensity applied to ambient so world outside isn't black
	diffuseTerm *= spotlight_Intensity;
	specularTerm *= spotlight_Intensity;

	// FragColour = Ambient term + Attenuation x (Diffuse term + Specular term)
	FragColour = ambientTerm + attenuation * (diffuseTerm + specularTerm);
}
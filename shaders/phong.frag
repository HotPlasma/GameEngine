#version 430

// Binds texture to model based on texture coordinates and applies Phong lighting to objects.

out vec4 FragColour;

in VS_OUT 
{
	vec2 TextureCoord;
	vec3 N;
	vec3 LightPosition;
	vec3 VertexPosition;
} vs_out;

uniform sampler2D tex;			//texture to be applied
uniform vec3 Ia;				//Ambient light intensity
uniform vec3 Id;				//Diffuse light intensity
uniform vec3 Is;				//Specular light intensity
uniform vec3 Rd;				//Diffuse reflectivity
uniform vec3 Rs;				//SPecular reflectivity
uniform float lightCutOff;
uniform float lightOuterCutOff;
uniform vec3 spotlightDirection;

void main()
{
	vec3 colour = texture(tex, vs_out.TextureCoord).rgb;						//Stores the colour of the textured pixel to be shaded
	vec3 lightVector = normalize(vs_out.LightPosition - vs_out.VertexPosition);	//Calculate the light vector
	
	//SPOTLIGHT CALCULATION
	float theta = dot(lightVector, normalize(-spotlightDirection));
	float epsilon = lightCutOff - lightOuterCutOff;
	float intensity = clamp((theta - lightOuterCutOff) / epsilon, 0.0, 1.0);

	//AMBIENT LIGHTING
	vec4 ambient = vec4((Ia * colour), 1.0);

		//ATTENUATION
		float d = length(vs_out.LightPosition - vs_out.VertexPosition);				//Calculate distance to light source for attenuation
		float attenuation = 1 - pow((d/400), 2);
		attenuation = clamp(attenuation, 0.0, 1.0);
			
		//DIFFUSE LIGHTING
		vec4 diffuseTemp = vec4(Id, 1.0) * max(dot(vs_out.N, lightVector), 0.0);
		diffuseTemp = clamp(diffuseTemp, 0.0, 1.0);
		vec4 diffuse = vec4(Rd, 1.0) * diffuseTemp;
		diffuse *= attenuation;
		
		//SPECULAR LIGHTING
		int Sre = 30;																//Create the specular reflection exponent
		vec4 specular = vec4(Is, 1.0) * vec4(Rs, 1.0f) * pow(max(dot(normalize(reflect(
		lightVector, normalize(vs_out.N))), normalize(vs_out.VertexPosition)), 0.0), Sre);
		specular *= attenuation;

		FragColour = ambient + (intensity * (diffuse + specular));
}
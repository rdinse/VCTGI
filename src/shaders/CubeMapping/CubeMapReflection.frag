#version 330 core

//incoming data
in vec4 passPosition;
in vec3 passNormal;
in vec2 passUVCoord;

in vec3 passReflectionNormal;

uniform struct 
{
    vec3 diffuse;
	vec3 specular;
    float shininess;
	float reflectionStrength;
} mat;

uniform int useColorTexture;
uniform sampler2D colorTexture; 

uniform struct 
{
  vec4 pos;    //pos.w = 0 dir. light, pos.w = 1 point light
  vec3 col;
  vec3 spot_direction; 
  float spot_exponent; 
  float spot_cutoff;  // no spotlight if cutoff = 0
} light;

uniform vec3 lightAmbient;

uniform samplerCube cubeMap;

uniform mat4 viewMatrix;

out vec4 fragmentColor;
 
void main(){
	///////////////////////////////////////////////////////
	///////////////// Cube Map Reflection /////////////////
	///////////////////////////////////////////////////////
	vec3 pos = normalize( vec3( passPosition ) );
	vec3 normal = normalize( passReflectionNormal);
	vec3 reflected = reflect( pos, normal);
	reflected = vec3( inverse(viewMatrix) * vec4( reflected, 0.0));
	
	vec3 inNormal = normalize( passNormal);
	
	//Diffuse
	vec3 lightVector;
	vec3 light_camcoord = (viewMatrix * light.pos).xyz;
    if (light.pos.w > 0.001f)
		lightVector = normalize( light_camcoord - passPosition.xyz);
    else
		lightVector = normalize(light_camcoord);
    float cos_phi = max( dot( inNormal, lightVector), 0.0f);
	
    // Specular
    vec3 eye = normalize( -passPosition.xyz);    
    vec3 reflection = normalize( reflect( -lightVector, inNormal));
    float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), mat.shininess);
	
	//Spotlight
	float spot = 1.0;
	if (light.spot_cutoff >= 0.001f)
	{
		float cos_phi_spot = max( dot( -lightVector, mat3(viewMatrix) * light.spot_direction), 0.0f);
		if( cos_phi_spot >= cos( light.spot_cutoff))
			spot = pow( cos_phi_spot, light.spot_exponent);
		else
			spot = 0.0f;
	}
	
	//Color
	vec3 diffuse_color = mat.diffuse;
	if (useColorTexture != 0)
		diffuse_color = texture( colorTexture, passUVCoord ).rgb;
	
	//All together
	fragmentColor.rgb = diffuse_color * lightAmbient;
	fragmentColor.rgb += spot * diffuse_color * cos_phi * light.col;
	fragmentColor.rgb += spot * mat.specular * cos_psi_n * light.col;
	fragmentColor *= (1.0 - mat.reflectionStrength);
	fragmentColor += texture( cubeMap, reflected) * mat.reflectionStrength;
	fragmentColor.a = 1.0;
}

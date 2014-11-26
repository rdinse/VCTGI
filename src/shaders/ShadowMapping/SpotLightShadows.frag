#version 330 core

#define HW_PCF

in vec3 passPosition;
in vec3 passNormal;
in vec2 passTcoord;
in vec4 passShadowCoord;

uniform mat4 viewMatrix;

uniform struct 
{
    vec3 diffuse;
	vec3 specular;
    float shininess;
	float alpha;
} mat;

uniform struct 
{
  vec4 pos;    //pos.w = 0 dir. light, pos.w = 1 point light
  vec3 col;
  vec3 spot_direction; 
  float spot_exponent; 
  float spot_cutoff;  
} light;

uniform vec3 lightAmbient;

uniform int useColorTexture;
uniform sampler2D colorTexture; 

uniform sampler2D shadowMap;

out vec4 fragmentColor;


void main() 
{ 
	vec3 lightVector;
	float spot;
	vec3 diffuse_color;
	float diffuse_alpha;

	// Diffuse
	vec3 light_camcoord = (viewMatrix * light.pos).xyz;
    if (light.pos.w > 0.001f)
		lightVector = normalize( light_camcoord - passPosition);
    else
		lightVector = normalize(light_camcoord);
    float cos_phi = max( dot( passNormal, lightVector), 0.0f);

    // Specular
    vec3 eye = normalize( -passPosition);    
    vec3 reflection = normalize( reflect( -lightVector, passNormal));
    float cos_psi_n = pow( max( dot( reflection, eye), 0.0f), mat.shininess);

    // Spotlight	
	float cos_phi_spot = max( dot( -lightVector, mat3(viewMatrix) * light.spot_direction), 0.0f);
	if( cos_phi_spot >= cos( light.spot_cutoff))
		spot = pow( cos_phi_spot, light.spot_exponent);
	else
		spot = 0.0f;

	// Textures 
	if (useColorTexture != 0)
	{
		diffuse_color = texture( colorTexture, passTcoord ).rgb;
		diffuse_alpha = texture( colorTexture, passTcoord ).a;
	}
	else
	{
		diffuse_color = mat.diffuse;
		diffuse_alpha = mat.alpha;
	}
	
	// Shadow
	float inShadow = 1.0;
	vec3 shadowCoord = passShadowCoord.xyz / passShadowCoord.w;

	//make a depth map lookup
    float lightDepth = texture(shadowMap, shadowCoord.xy).z;

    //test if the fragment is visible by comparing the z-values of the 
    //lightmap and the projection considering a bias (e.g. 0.0005)
    if (lightDepth < shadowCoord.z - 0.00005) 
        inShadow = 0.3;    

	// All together 
	fragmentColor.rgb = diffuse_color * lightAmbient;
	fragmentColor.rgb += spot * diffuse_color * cos_phi * light.col;
	fragmentColor.rgb += spot * mat.specular * cos_psi_n * light.col;
	fragmentColor.rgb *= inShadow;
	fragmentColor.a = diffuse_alpha;
}
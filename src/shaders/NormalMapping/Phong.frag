#version 330 core

in vec3 passPosition;
in vec3 passNormal;
in vec2 passTcoord;

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
  float spot_cutoff;  // no spotlight if cutoff = 0
} light;

uniform struct FogParams
{ 
   vec3 col; // Fog color
   float start; // This is only for linear fog
   float end; // This is only for linear fog
   float density; // For exp and exp2 equation  
   int mode; // 1 = linear, 2 = exp, 3 = exp2
} fog;

uniform vec3 lightAmbient;

uniform int useColorTexture;
uniform sampler2D colorTexture; 

out vec4 fragmentColor;

float getFogFactor(FogParams fog, float z) 
{ 
   float f = 0.0; 
   if (fog.mode == 1) 
      f = (fog.end-z)/(fog.end-fog.start); 
   else if (fog.mode == 2) 
      f = exp(-fog.density*z); 
   else if( fog.mode == 3) 
      f = exp(-pow(fog.density*z, 2.0)); 
       
   f = clamp(f, 0.0, 1.0); 
     
   return f; 
}

void main() 
{ 
	vec3 lightVector;
	float spot;
	vec3 diffuse_color;
	float diffuse_alpha;

	// Diffuse
	vec3 light_camcoord = (light.pos).xyz;
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
	if (light.spot_cutoff < 0.001f)
		spot = 1.0;
	else
		{
		float cos_phi_spot = max( dot( -lightVector, mat3(viewMatrix) * light.spot_direction), 0.0f);
		if( cos_phi_spot >= cos( light.spot_cutoff))
			spot = pow( cos_phi_spot, light.spot_exponent);
		else
			spot = 0.0f;
		}

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

	// All together 
	fragmentColor.rgb = diffuse_color * lightAmbient;
	fragmentColor.rgb += spot * diffuse_color * cos_phi * light.col;
	fragmentColor.rgb += spot * mat.specular * cos_psi_n * light.col;
	fragmentColor.a = diffuse_alpha;


	//Add fog
	if (fog.mode != 0)
	{
		float f = getFogFactor(fog, length( passPosition));
        fragmentColor.rgb = f * fragmentColor.rgb + (1-f) * fog.col;
	}
}
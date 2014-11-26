#version 330 core

uniform sampler2D colortexture;
uniform sampler2D normaltexture;

uniform struct 
{   
    vec3 diffuse;
    vec3 specular;
    float shininess;
} mat;

uniform struct 
{
  vec4 pos; // light position in world coordinates 
  vec3 col; // light color
} light;

uniform vec3 lightAmbient;
uniform int useColorTexture;


in vec3 passLightVector;
in vec3 passEyeVector;
in vec2 passUV;

out vec4 fragmentColor;

void main(){
    
    /***************** Diffuse ******************************************************/
    // local normal, in tangent space
    vec3 normal = texture(normaltexture,passUV).rgb * 2.0 - 1.0;

    // direction of the light (from the fragment to the light) in tangent space
    vec3 lightVector = normalize(passLightVector);

    //compute the diffuse lighting factor
    float cos_phi = max(dot(normal, lightVector), 0);

    /***************** Specular *****************************************************/
    // compute the normalized reflection vector using GLSL's built-in reflect() function
    vec3 reflection = normalize(reflect(-lightVector, normal));

    // eye vector in tangent space
    vec3 eyeVector = normalize(passEyeVector);

    // compute the specular lighting factor
    float cos_psi_n = pow(max(dot(reflection, eyeVector), 0.0f), mat.shininess);

    /***************** Material properties ******************************************/
    vec3 diffuse_color;
    if (useColorTexture != 0)
        diffuse_color = texture(colortexture, passUV).rgb;
    else
        diffuse_color = mat.diffuse;
    
    /***************** All together *************************************************/
    fragmentColor.rgb = diffuse_color * lightAmbient;
    fragmentColor.rgb += diffuse_color * cos_phi * light.col;
    fragmentColor.rgb += mat.specular * cos_psi_n * light.col;
    fragmentColor.a = 1.0;

}
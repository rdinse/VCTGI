#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform struct 
{
  vec4 pos; // light position in world coordinates 
  vec3 col; // light color
} light;

out vec3 passLightVector;
out vec3 passEyeVector;
out vec2 passUV;

void main(){
    
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    /*********** Calculate matrix that goes from camera space to tangent space **********/
    mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
	vec3 normal_cs = normalize((normalMatrix * vec4(normal,0)).xyz);
	vec3 tangent_cs = normalize((normalMatrix * vec4(tangent,0)).xyz);
	vec3 bitangent_cs = cross(tangent_cs, normal_cs);

    mat3 tbn = transpose(mat3(
        tangent_cs,
        bitangent_cs,
        normal_cs
    ));

    /*********** Calculate position in camera space *************************************/
	vec3 position_cs = (viewMatrix * modelMatrix * position).xyz;

    /*********** Calculate light vector (tangent space) *********************************/
    // light position in world coordinates
	vec3 lightPosition_cs = (light.pos).xyz;

    // calcluate vector that goes from the vertex to the light, in tangent space
	passLightVector = tbn * normalize(lightPosition_cs - position_cs);

    /*********** Calculate eye vector (tangent space) **********************************/
    // calculate eye vector in camera space
    vec3 eye_cs = normalize(-position_cs);

    // calculate eye vector in tangent space
    passEyeVector = tbn * eye_cs;

    /*********** Pass uv of the vertex *************************************************/
	// no special space for this one
	passUV = uv;

}
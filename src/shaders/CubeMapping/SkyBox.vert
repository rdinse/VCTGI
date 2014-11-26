#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passTexCoord;
 
void main(){
	passTexCoord = position.xyz;

	mat4 vM = mat4(mat3(viewMatrix));

    gl_Position = projectionMatrix * vM * position;
}

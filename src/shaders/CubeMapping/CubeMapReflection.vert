#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec3 passNormal;
out vec2 passUVCoord;

out vec3 passReflectionNormal;
 
void main(){
	
    passPosition = viewMatrix * modelMatrix * position;
    passNormal = vec3(transpose(inverse(viewMatrix * modelMatrix)) * vec4(normal, 0));
    passUVCoord = uv;
	
    passReflectionNormal = vec3( viewMatrix * modelMatrix * vec4(normal, 0));
	
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}

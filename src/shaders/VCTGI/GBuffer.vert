#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangent;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec3 passNormal;
out vec2 passUVCoord;
out vec3 passTangent;
 
void main(){
	passPosition = modelMatrix * position;
    passNormal = vec3(transpose(inverse(modelMatrix)) * vec4(normal, 0));
    //passPosition = position;
	//passNormal = normal;
    passUVCoord = uv;
    passTangent = tangent;
	
	
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}

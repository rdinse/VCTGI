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

 
void main(){
    passUVCoord = uv;

    passPosition = viewMatrix * modelMatrix * position;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    passNormal = vec3(transpose(inverse(viewMatrix * modelMatrix)) * vec4(normal, 0));
}

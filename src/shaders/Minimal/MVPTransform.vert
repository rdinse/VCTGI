#version 330

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passNormal;

void main() {
	//transform the normal correctly into view space 
	//and pass it to the primitive assembly and the rasterizer
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    //pass the normal to the fragment shader
    passNormal = normal;
}
 

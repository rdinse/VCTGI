#version 330

/*
	The Vertex Shader is absolutely trivial. We are only interested in the Vertex depth. No fancy stuff here!
*/

layout (location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
 

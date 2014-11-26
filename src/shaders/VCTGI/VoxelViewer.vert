#version 430

layout (location = 0) in vec3 positionAttribute;
layout (location = 1) in vec3 texAttribute;

out vec3 texCoords;

void main() {
    gl_Position = vec4(positionAttribute, 1.0);
    texCoords = texAttribute;
}
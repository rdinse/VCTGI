#version 330

in vec2 passUVCoord;

uniform sampler2D colortexture;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(colortexture, passUVCoord);
}
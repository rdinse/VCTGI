#version 330 core

in vec3 passNormal;
out vec4 fragmentColor;

void main(){
    fragmentColor = vec4(abs(passNormal),1);
}
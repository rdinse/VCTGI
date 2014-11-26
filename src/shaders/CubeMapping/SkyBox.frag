#version 330 core

//incoming data
in vec3 passTexCoord;

uniform samplerCube cubeMap;

out vec4 fragmentColor;
 
void main(){
	fragmentColor = texture(cubeMap, passTexCoord);
}

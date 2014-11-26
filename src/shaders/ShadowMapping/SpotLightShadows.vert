#version 330

layout (location = 0) in vec4 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Tcoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform mat4 lightVPBias;

out vec3 passPosition;
out vec3 passNormal;
out vec2 passTcoord;
out vec4 passShadowCoord;


void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * Position;

    //transform the position correctly into view space
    //and pass it to the fragment shader
    passPosition = (viewMatrix * modelMatrix * Position).xyz;

    //transform the normal correctly into view space 
    //and pass it to the fragment shader
	mat3 normalMatrix = mat3( transpose( inverse( viewMatrix * modelMatrix)));
    passNormal = normalize( normalMatrix * Normal);
	passTcoord = Tcoord;
	
	passShadowCoord = lightVPBias * modelMatrix * Position;
}

#version 330 core

//incoming data for the single textures
in vec4 passPosition;
in vec3 passNormal;
in vec2 passUVCoord;

uniform vec3 diffuseColor;
uniform float alpha;

uniform int useColorTexture;
uniform sampler2D colortexture;

//writable textures for deferred screen space calculations
layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;
 
void main(){  
    positionOutput = passPosition;
    normalOutput = vec4(normalize(passNormal), 1);
	colorOutput = vec4(diffuseColor, 1);

	if (useColorTexture != 0)
	{
		colorOutput = texture(colortexture, passUVCoord);
	}
}

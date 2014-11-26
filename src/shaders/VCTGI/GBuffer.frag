#version 330 core

//incoming data for the single textures
in vec4 passPosition;
in vec3 passNormal;
in vec2 passUVCoord;
in vec3 passTangent;

uniform vec3 diffuseColor;
uniform float alpha;

uniform int useColorTexture;
uniform sampler2D colorTexture;

//writable textures for deferred screen space calculations
layout(location = 0) out vec4 positionOutput;
layout(location = 1) out vec4 normalOutput;
layout(location = 2) out vec4 colorOutput;
layout(location = 3) out vec4 tangentOutput;
 
void main(){  
    positionOutput = passPosition;
    normalOutput = vec4(normalize(passNormal), 0.0);
    tangentOutput = vec4(normalize(passTangent), 0.0);

	if (useColorTexture != 0) {
		colorOutput = texture(colorTexture, passUVCoord);
	} else {
		colorOutput = vec4(diffuseColor, 1);
	}
}

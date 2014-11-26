#version 330

in vec2 passUVCoord;

uniform sampler2D lightMap;
uniform sampler2D colorMap;
uniform sampler2D ssaoMap;

uniform float useLight;
uniform float useSSAO;

out vec4 fragmentColor;

void main() {
    vec4 light = texture(lightMap, passUVCoord);
    vec4 color = texture(colorMap, passUVCoord);
    vec4 ssao = texture(ssaoMap, passUVCoord);

    fragmentColor = color;
	if (useLight > 0.5) 
		fragmentColor *= light;
	if(color.a < 0.5) fragmentColor = vec4(0.4,0.4,0.4,1.0);
	if (useSSAO > 0.5) 
		fragmentColor *= (ssao * 0.7 + 0.3);
    //fragmentColor = color;
}
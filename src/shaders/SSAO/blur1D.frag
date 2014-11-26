#version 330

in vec2 passUVCoord;

uniform sampler2D colortexture;
uniform float yAxis;

out vec4 fragmentColor;

void main() {
	float weights[4] = {2.0, 3.0, 2.0, 1.0};
    
	vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);
	vec2 diff = vec2(1.0, 0.0);
	if (yAxis > 0.5) diff = vec2(0.0, 1.0);
	diff *= (1.0 / 800.0);
	
	for(int i=0; i<4; i++)
	{
		sum += weights[i] * texture(colortexture, passUVCoord + i*diff);
		sum += weights[i] * texture(colortexture, passUVCoord - i*diff);
	}
	
	fragmentColor = sum / 16.0;
}
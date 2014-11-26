#version 330

layout (location = 0) in vec4 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec4 midPosition;

out vec4 color;

void main() {
	vec3 c = ( modelMatrix * vec4(0,0,0,1)).xyz;
	float x = c.x + 2;
	float y = c.y + 2;
	float z = c.z + 2;
	c = vec3( x*2.3+y*5.6+z*4.4, x*3.1+y*7.2+z, x*4.3+y+z*6.9);
	c = sin(c);
	c = c/2.0 + 1.0;
	c = normalize(c);
	color = vec4( c, 1.0);

    passPosition = viewMatrix * modelMatrix * position;
    midPosition = viewMatrix * modelMatrix * vec4(0,0,0,1);
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
}
#version 430

layout (location = 0) in vec4 positionAttribute;
layout (location = 1) in vec3 normalAttribute;
layout (location = 2) in vec2 uvCoordinateAttribute;

out vertPassS {
  vec3 normal;
  vec2 uv;
} vertPass;

uniform mat4 uniformModelMatrix;

void main() {
  gl_Position = uniformModelMatrix * positionAttribute;
  vertPass.normal = (inverse(transpose(uniformModelMatrix)) * vec4(normalAttribute, 0.0)).xyz;
  vertPass.uv = uvCoordinateAttribute;
}

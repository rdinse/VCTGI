#version 430

in vec2 passUVCoord;

uniform sampler2D colortexture;
layout(binding = 1) uniform sampler2D depthtexture;
layout(binding = 2) uniform sampler2D normaltexture;

out vec4 fragmentColor;

const float twoSigmaSquared = 0.14;
float similarity(vec4 a, vec4 b, float scale) {
  float d = scale * length(a - b);
  return exp(-(d * d/ twoSigmaSquared));
}

void main() {
  ivec2 uvCoordF = ivec2(passUVCoord * vec2(800, 600));  // TODO resolution via uniform
  vec2 texCoordOffset = vec2(1.0 / 800.0, 1.0 / 600.0);
  
  vec3 luma = vec3(0.299, 0.587, 0.144);
  float lumaSample1 = dot(luma, texture2D(colortexture, passUVCoord.xy + (vec2(0, 0) * texCoordOffset)).xyz);
  float lumaSample2 = dot(luma, texture2D(colortexture, passUVCoord.xy + (vec2(1, 1) * texCoordOffset)).xyz);
  float lumaSample3 = dot(luma, texture2D(colortexture, passUVCoord.xy + (vec2(-1, 1) * texCoordOffset)).xyz);
  float lumaSample4 = dot(luma, texture2D(colortexture, passUVCoord.xy + (vec2(1, -1) * texCoordOffset)).xyz);
  float lumaSample5 = dot(luma, texture2D(colortexture, passUVCoord.xy + (vec2(-1, -1) * texCoordOffset)).xyz);
  
  vec2 dir;
  dir.x = -((lumaSample5 + lumaSample4) - (lumaSample3 + lumaSample2));
  dir.y = ((lumaSample5 + lumaSample3) - (lumaSample4 + lumaSample2));
  
  float weakening = max((lumaSample3 + lumaSample2 + lumaSample4 + lumaSample5) / (8.0 * 4.0), 0.01);
  float scalar = 1.0 / (weakening + min(abs(dir.x), abs(dir.y)));
  dir = clamp(dir * scalar, -vec2(8.0), vec2(8.0)) * texCoordOffset;
  
  vec3 result1 = (texture2D(colortexture, passUVCoord.xy + dir * vec2(-0.11)).xyz +
                  + texture2D(colortexture, passUVCoord.xy + dir * vec2(0.11)).xyz) / 2.0;

  vec3 result2 = result1 / 2.0 + (texture2D(colortexture, passUVCoord.xy + dir * vec2(-0.5)).xyz +
                  + texture2D(colortexture, passUVCoord.xy + dir * vec2(0.5)).xyz) / 4.0;

  float lumaMin = min(lumaSample1, min(lumaSample2, min(lumaSample3, min(lumaSample4, lumaSample5))));
  float lumaMax = max(lumaSample1, max(lumaSample2, max(lumaSample3, max(lumaSample4, lumaSample5))));
  float lumaResult = dot(luma, result2);

  if (lumaResult < lumaMin || lumaResult > lumaMax) {
    fragmentColor = vec4(result1, 1.0);
  } else {
    fragmentColor = vec4(result2, 1.0);
  }
}

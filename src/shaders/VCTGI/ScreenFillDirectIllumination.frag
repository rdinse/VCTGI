#version 430

in vec2 passUVCoord;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

layout(binding = 3) uniform sampler3D voxelTex[6];

uniform mat4 uniformWorldToVoxelsMatrix;
uniform int  uniformVoxelDepth;
uniform int  uniformVoxelRes;

uniform vec3 uniformCameraPos;

uniform mat4 uniformLightMVP;
uniform vec3 uniformLightPos;
uniform vec3 uniformLightColor;
uniform float uniformShadowMapBias;

out vec4 fragmentColor;

float traceCone(vec3 pos, vec3 dir, float aperture, float maxTracingDistance) {

  uvec3 visibleFace;
  visibleFace.x = (dir.x < 0.0) ? 0 : 1;
  visibleFace.y = (dir.y < 0.0) ? 2 : 3;
  visibleFace.z = (dir.z < 0.0) ? 4 : 5;
  
  vec3 weight = abs(dir);
  float voxelSize = 1.0 / uniformVoxelRes;
  float dst = voxelSize * 2;
  float diameter = aperture * dst;
  vec3 samplePos = dir * dst + pos;
  float shadowSample = 0.0;

  while (shadowSample <= 1.0 && dst <= maxTracingDistance) {
    
    if (aperture < 0.3 && (samplePos.x < 0 || samplePos.y < 0 || samplePos.z < 0
                       || samplePos.x > 1 || samplePos.y > 1 || samplePos.z > 1)) {
      
      break;
    }
    
    float mipLevel = max(log2(diameter * uniformVoxelRes), 0);

    vec4 interpolatedSample = weight.x * textureLod(voxelTex[visibleFace.x], samplePos, mipLevel)
                            + weight.y * textureLod(voxelTex[visibleFace.y], samplePos, mipLevel)
                            + weight.z * textureLod(voxelTex[visibleFace.z], samplePos, mipLevel);
        
    shadowSample += (1 - shadowSample) * interpolatedSample.a;
    dst += max(diameter, voxelSize);
    diameter = dst * aperture;
    samplePos = dir * dst + pos;
  }
  
  return shadowSample;
}

void main() {
  const bool coneTraceShadows = false;

  vec4 pos = texture(positionMap, passUVCoord);
  vec4 normalInterp = texture(normalMap, passUVCoord);
  vec3 normal = normalize(normalInterp.xyz);

  float shadowSample = 0.0;

  if (coneTraceShadows) {
    vec4 voxelPos_tmp = texture(positionMap, passUVCoord);
    voxelPos_tmp = uniformWorldToVoxelsMatrix * voxelPos_tmp;
    vec3 voxelPos = voxelPos_tmp.xyz / voxelPos_tmp.w;
    
    vec4 lightPos_tmp = uniformWorldToVoxelsMatrix * vec4(uniformLightPos.xyz, 1);
    vec3 lightPos = lightPos_tmp.xyz / lightPos_tmp.w;

    vec3 lightDir = lightPos - voxelPos;
    float lightDistance = length(lightDir);
    lightDir = normalize(lightDir);

    shadowSample = max(0.0, 1.0 - traceCone(voxelPos, lightDir, 0.21, lightDistance));
  } else {
    // Shadow map lookup
    vec4 shadowPos = uniformLightMVP * (pos + vec4(normal, 0) * 100.0 * uniformShadowMapBias);
    shadowPos = shadowPos / shadowPos.w;
    shadowPos.xyz = shadowPos.xyz * .5 + .5;
    
    float sum = 0;
    const float pcfD = 0.0006;
    for (int x = -2; x <= 2; x++) {
      for (int y = -2; y <= 2; y++) {
        float shadowSample = texture(shadowMap, shadowPos.xy + vec2(x,y) * pcfD).x;
        if (-shadowSample - uniformShadowMapBias + shadowPos.z < 0) sum++;
      }
    }
    shadowSample = sum / 25.0;
  }
    
  // Diffuse term
  /* vec3 normalizedNormal = normalize(normal.xyz); */
  /* vec3 lightDir = -pos.xyz + uniformLightPos; */
  /* float cos_phi = clamp(dot(normalizedNormal, lightDir), 0.0, 1.0); */
  /* vec3 diffuseLight = cos_phi * uniformLightColor; */
  
  /* // Attenuation */
  /* float distance = length(cameraToLight); */
  // float attenuation = 1.0 / (distance * distance);

  vec3 lightDir = normalize(uniformLightPos - pos.xyz);
  float lambertian = max(dot(lightDir, normal), 0.0);
  float specular = 0;

  if(lambertian > 0 && shadowSample > 0) {
    vec3 cameraToP = vec3(-pos.xyz + uniformCameraPos);
    vec3 viewDir = normalize(cameraToP);
      
    vec3 halfDir = normalize(lightDir + viewDir);
    float specularAngle = max(dot(halfDir, normal), 0.0);
    specular = pow(specularAngle, 10);
  }

  fragmentColor = vec4((shadowSample + 0.04) * uniformLightColor * (lambertian + specular), 1.0);
  
  // Specular term
  // vec3 specularLight = 0.1 * cos_phi * uniformLightColor * pow(max(0, dot(reflect(-lightDir, normalizedNormal), viewDir)), 3);
	    
  //fragmentColor = vec4(shadowSample * (diffuseLight + specularLight), 1.0);
}

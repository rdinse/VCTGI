#version 430

/* Stats:
   6 diffuse cones only: 0.71ms
   0.041 cone aperture, no skipping: 8.51ms
   0.041 cone aperture, with skipping: 
 */

in vec2 passUVCoord;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D tangentMap;
layout(binding = 3) uniform sampler3D voxelTex[6];

// uniform vec3 uniformLightPosition;
uniform vec4 uniformEyePos;

uniform float maxTracingDistance;

uniform float uniformAOAlpha;
uniform float uniformAOFalloff;

uniform float uniformSpecularAlpha;
uniform float uniformSpecularAperture;

uniform mat4 uniformWorldToVoxelsMatrix;
uniform mat4 uniformWorldToVoxelsNormalMatrix;
uniform int  uniformVoxelDepth;
uniform int  uniformVoxelRes;

layout(location = 0) out vec4 globalIllumination;
layout(location = 1) out vec4 ambientOcclusion;

const vec3 normals[6] = vec3[6] (vec3( 1.0,  0.0,  0.0),
                                 vec3(-1.0,  0.0,  0.0),
                                 vec3( 0.0,  1.0,  0.0),
                                 vec3( 0.0, -1.0,  0.0),
                                 vec3( 0.0,  0.0,  1.0),
                                 vec3( 0.0,  0.0, -1.0));

/*
float intersectPlane(vec3 n, vec3 support_p, vec3 dir, vec3 support_l) {
  float d = dot(n, support_p - support_l) / dot(n, dir);
  if (d < 0) {
    return 1.0 / 0.0;
  } else {
    return d;
  }
  }*/
void intersectVoxel (vec3 bMin, vec3 bMax, vec3 rOrig, vec3 rDir, out float tNear, out float tFar) {
    vec3 inverseR = 1.0 / rDir;
    vec3 tTop = inverseR * (bMax - rOrig);
    vec3 tBot = inverseR * (bMin - rOrig);
    
    vec3 tMin = min(tTop, tBot);
    vec3 tMax = max(tTop, tBot);
    
    vec2 t = max(tMin.xx, tMin.yz);
    tNear = max(t.x, t.y);
    t = min(tMax.xx, tMax.yz);
    tFar = min(t.x, t.y);
}

vec4 traceCone(vec3 pos, vec3 dir, float aperture, bool aoEnabled) {
  // Assume vector dir is normalized and aperture is the cone diameter at a distance of 1.
  
  aoEnabled = aoEnabled && uniformAOAlpha > 0;

  uvec3 visibleFace;
  visibleFace.x = (dir.x < 0.0) ? 0 : 1;
  visibleFace.y = (dir.y < 0.0) ? 2 : 3;
  visibleFace.z = (dir.z < 0.0) ? 4 : 5;
	
  vec3 weight = abs(dir);
	
  float voxelSize = 1.0 / uniformVoxelRes;
  vec4 coneSample = vec4(0.0);
  float aoSample = 0;
  float dst = voxelSize*2;  // Offset to prevent self-intersection.
  float diameter = aperture * dst;
  vec3 samplePos = dir * dst + pos;
  int emptyCount = 0;
  
  while (coneSample.a <= 1.0 && dst <= maxTracingDistance) {
    
    // FIXME: "<" results in discontinuities, fade instead.
    if (aperture < 0.3 && (samplePos.x < 0 || samplePos.y < 0 || samplePos.z < 0
                       || samplePos.x > 1 || samplePos.y > 1 || samplePos.z > 1)) {
      
      break;
    }
    
    float mipLevel = max(log2(diameter * uniformVoxelRes), 0);

    vec4 interpolatedSample = weight.x * textureLod(voxelTex[visibleFace.x], samplePos, mipLevel)
                            + weight.y * textureLod(voxelTex[visibleFace.y], samplePos, mipLevel)
                            + weight.z * textureLod(voxelTex[visibleFace.z], samplePos, mipLevel);
    
    coneSample += (1 - coneSample.a) * interpolatedSample;
    
    if (aoEnabled && aoSample <= 1.0) {
      aoSample += ((1 - aoSample) * interpolatedSample.a) / (1 + dst * uniformAOFalloff);
    }
    
    // Attempt to skip empty regions.
    if (interpolatedSample.a < 0.01) {
      emptyCount++;
    }
    
    float samplingStepSize = 0;
    if (!aoEnabled && emptyCount > 1 && interpolatedSample.a < 0.01 && mipLevel < uniformVoxelDepth - 3) {
      
      /*
      // Bottom-up.
      int maxLevel = int(ceil(mipLevel));
      int maxLevelRes = 1 << (uniformVoxelDepth - maxLevel);
      ivec3 maxVoxelPosI = min(ivec3(maxLevelRes * pos), ivec3(maxLevelRes - 1));
      
      // Find non-empty *parent voxel.
      while (maxLevel < uniformVoxelDepth - 2 && texelFetch(voxelTex[0], maxVoxelPosI, maxLevel).a < 0.01) {
        maxLevel++;
        maxLevelRes >>= 1;
        maxVoxelPosI = min(ivec3(maxLevelRes * pos), ivec3(maxLevelRes - 1));
      }
      
      // Go back one level below non-empty level.
      maxLevel--;
      maxLevelRes <<= 1;
      maxVoxelPosI = min(ivec3(maxLevelRes * pos), ivec3(maxLevelRes - 1));
      */

      // Top-down.
      int maxLevel = uniformVoxelDepth - 3;
      int maxLevelRes = 1 << (uniformVoxelDepth - maxLevel);
      ivec3 maxVoxelPosI = min(ivec3(maxLevelRes * samplePos), ivec3(maxLevelRes - 1));
      
      // Find first empty *parent voxel.
      while (maxLevel > 0 && texelFetch(voxelTex[0], maxVoxelPosI, maxLevel).a > 0.01) {
        maxLevel--;
        maxLevelRes <<= 1;
        maxVoxelPosI = min(ivec3(maxLevelRes * samplePos), ivec3(maxLevelRes - 1));
      }
      
      // Intersect ray with back-planes of voxel.
      float maxVoxelSize = 1.0 / float(maxLevelRes);
      //vec3 maxVoxelPos = vec3(maxVoxelPosI) / float(maxLevelRes);
      vec3 maxVoxelPos =  (1.0 / float(maxLevelRes)) * vec3(maxVoxelPosI);
      float tNear, tFar;
      intersectVoxel(maxVoxelPos,
                     maxVoxelPos + vec3(maxVoxelSize),
                     samplePos, dir, tNear, tFar);
      
      /*
        float halfMaxVoxelSize = 0.5 / float(maxLevelRes);
        vec3 maxVoxelPos = vec3(maxVoxelPosI) / float(maxLevelRes) + vec3(halfMaxVoxelSize);
        float xDst = intersectPlane(normals[visibleFace.x], maxVoxelPos - halfMaxVoxelSize * normals[visibleFace.x], dir, samplePos);
        float yDst = intersectPlane(normals[visibleFace.y], maxVoxelPos - halfMaxVoxelSize * normals[visibleFace.y], dir, samplePos);
        float zDst = intersectPlane(normals[visibleFace.z], maxVoxelPos - halfMaxVoxelSize * normals[visibleFace.z], dir, samplePos);
        samplingStepSize = min(xDst, min(yDst, zDst)) + voxelSize / 2.0;
      */
      
      samplingStepSize = tFar + voxelSize / 2.0;
      
      // debugging
      //if (tNear > 0) coneSample = vec4(1,1,1,1.1);
      //if (tFar < -0.2) coneSample = vec4(1,1,1,1.1);
      //if (!aoEnabled && abs(maxVoxelPos.x - (pos + dir * (tFar + dst)).x) > 0.1) coneSample = vec4(1, 0, 0, 1.1);

      emptyCount = 0;
    } else {
      samplingStepSize = diameter;
    }

    dst += max(samplingStepSize, voxelSize);
    diameter = dst * aperture;
    samplePos = dir * dst + pos;
  }
  
  if (aoEnabled) {
    ambientOcclusion -= vec4(aoSample);
  }
	
  return coneSample;
}

void main() {
  ambientOcclusion = vec4(1.0);
  
  ivec2 uvCoordI = ivec2(passUVCoord * vec2(800, 600));

  vec4 worldPos = texelFetch(positionMap, uvCoordI, 0);
  worldPos = uniformWorldToVoxelsMatrix * worldPos;
  vec3 pos = worldPos.xyz / worldPos.w;
  
  vec4 normal = vec4(texelFetch(normalMap, uvCoordI, 0).xyz, 0);
  normal = normalize(normal);
  vec3 tangent = texelFetch(tangentMap, uvCoordI, 0).xyz;
  tangent = normalize(tangent);
  vec3 bitangent = cross(tangent, normal.xyz);
  const mat3 tangentSpaceMat = mat3(tangent, normal.xyz, bitangent);
  
  // Shift ray origins to avoid self-intersection.
  pos += 2 * normal.xyz / uniformVoxelRes;

  vec4 diffuse = vec4(0.0);
  vec4 specular = vec4(0.0);
  if (uniformSpecularAlpha > 0.0) {
    vec3 eyePos = (uniformWorldToVoxelsMatrix * vec4(uniformEyePos.xyz, 1)).xyz;
    vec3 eyeToFragment = vec3(pos - eyePos);
    vec3 viewDir = normalize(eyeToFragment);
    //viewDir = vec3(1,0,0);
    vec3 specularDir = reflect(viewDir, normal.xyz);
    specular = traceCone(pos, specularDir, uniformSpecularAperture, false);
  }

  // The six cones are approximately arranged on the sides of one half of a dodecahedron.
  // The weight is the factored out cosine of the integral (1.0 for the center cone).
  diffuse +=         traceCone(pos, normal.xyz,                                             2 * 0.5,      true);
  diffuse += 0.607 * traceCone(pos, tangentSpaceMat * vec3(-0.794654, 0.607062,  0.0     ), 2 * 0.549092, true);
  diffuse += 0.607 * traceCone(pos, tangentSpaceMat * vec3( 0.642889, 0.607062,  0.467086), 2 * 0.549092, true);
  diffuse += 0.607 * traceCone(pos, tangentSpaceMat * vec3( 0.642889, 0.607062, -0.467086), 2 * 0.549092, true);
  diffuse += 0.607 * traceCone(pos, tangentSpaceMat * vec3(-0.245562, 0.607062,  0.755761), 2 * 0.549092, true);
  diffuse += 0.607 * traceCone(pos, tangentSpaceMat * vec3(-0.245562, 0.607062, -0.755761), 2 * 0.549092, true);
  

  globalIllumination = vec4(diffuse.rgb + uniformSpecularAlpha * specular.rgb, 1.0) + vec4(gl_SampleID) * 0.0000001; //  FIXME: enable SSAA the proper way.
  ambientOcclusion = clamp(ambientOcclusion, 0, 1);
}

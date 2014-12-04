#version 430

in geomPassS {
  vec3 worldPos;
  vec3 voxelPos;
  vec3 normal;
  vec2 uv;
} geomPass;

layout(binding = 0, r32ui) uniform uimage3D voxelMaps[6];

uniform bool useColorTexture;
uniform vec3 diffuseColor;
uniform sampler2D diffuseTexture;

uniform mat4 uniformLightMVP;
uniform vec3 uniformLightPos;
uniform vec3 uniformLightColor;
uniform sampler2D shadowMap;
uniform float uniformShadowMapBias;

uniform int uniformVoxelRes;

flat in vec4 triangleBoundingBox;

out vec4 dummyOut;


// Auxiliary functions borrowed from OpenGL Insights, 2011

uint convVec4ToRGBA8(vec4 val) {
  return (uint(val.w) & 0x000000FF) << 24U
    | (uint(val.z) & 0x000000FF) << 16U
    | (uint(val.y) & 0x000000FF) << 8U
    | (uint(val.x) & 0x000000FF);
}

vec4 convRGBA8ToVec4(uint val) {
  return vec4(float((val & 0x000000FF)),
              float((val & 0x0000FF00) >> 8U),
              float((val & 0x00FF0000) >> 16U),
              float((val & 0xFF000000) >> 24U));
}

/*
void imageAtomicRGBA8Avg(layout(r32ui) uimage3D img,
                         ivec3 coords, vec4 val3) {

  // Alpha channel is used a the sample counter of the moving average.

  vec4 val = val3;//vec4(val3, 1);
  val.rgb *= 255.0f;
  uint newVal = convVec4ToRGBA8(val);
  uint prevStoredVal = 0;
  uint currStoredVal;

  // Loop as long as destination value gets changed by other threads
  while ((currStoredVal = imageAtomicCompSwap(img, coords, prevStoredVal, newVal))
         != prevStoredVal) {

    vec4 rval = convRGBA8ToVec4(currStoredVal);
    rval.rgb = (rval.rgb * rval.a);  // Denormalize
    vec4 curValF = rval + val;       // Add new value
    curValF.rgb /= (curValF.a);      // Renormalize
    newVal = convVec4ToRGBA8(curValF);

    prevStoredVal = currStoredVal;
  }
}
*/

uint encUnsignedNibble(uint m, uint n) {
  return (m & 0xFEFEFEFE)
    | (n & 0x00000001)
    | (n & 0x00000002) << 7U
    | (n & 0x00000004) << 14U
    | (n & 0x00000008) << 21U;
}

uint decUnsignedNibble(uint m) {
  return (m & 0x00000001)
    | (m & 0x00000100) >> 7U
    | (m & 0x00010000) >> 14U
    | (m & 0x01000000) >> 21U;
}

void imageAtomicRGBA8Avg(layout(r32ui) uimage3D img,
                         ivec3 coords, vec4 val) {

  // LSBs are used for the sample counter of the moving average.

  val *= 255.0;
  uint newVal = encUnsignedNibble(convVec4ToRGBA8(val), 1);
  uint prevStoredVal = 0;
  uint currStoredVal;

  int counter = 0;
  // Loop as long as destination value gets changed by other threads
  while ((currStoredVal = imageAtomicCompSwap(img, coords, prevStoredVal, newVal))
         != prevStoredVal && counter < 16) {

    vec4 rval = convRGBA8ToVec4(currStoredVal & 0xFEFEFEFE);
    uint n = decUnsignedNibble(currStoredVal);
    rval = rval * n + val;
    rval /= ++n;
    rval = round(rval / 2) * 2;
    newVal = encUnsignedNibble(convVec4ToRGBA8(rval), n);

    prevStoredVal = currStoredVal;

    counter++;
  }
}

void main() {
  if (any(lessThan(gl_FragCoord.xy, triangleBoundingBox.xy))
      || any(greaterThan(gl_FragCoord.xy, triangleBoundingBox.zw))) {

    discard;  // Clip residue corners from conservative rasterization.
  }

  vec3 diffuse;
  if (useColorTexture) {
    diffuse = texture(diffuseTexture, geomPass.uv).rgb;
  } else {
    diffuse = diffuseColor;
  }

  vec4 shadowPos = uniformLightMVP * vec4(geomPass.worldPos, 1.0);
  shadowPos = shadowPos / shadowPos.w;
  shadowPos.xyz = shadowPos.xyz * .5 + .5;
  float shadowSample = texture2D(shadowMap, shadowPos.xy).x;

  if (-shadowSample - uniformShadowMapBias + shadowPos.z < 0) {
    shadowSample = 1.0;
  } else {
    shadowSample = 0.07;
  }

  vec3 normalizedNormal = normalize(geomPass.normal);
  vec3 lightDir = - geomPass.worldPos + uniformLightPos;
  float cos_phi = clamp(dot(normalizedNormal, lightDir), 0.0, 1.0);
  vec3 diffuseLight = cos_phi * shadowSample * uniformLightColor;

  vec3 radiance = diffuse * diffuseLight;
  ivec3 voxelPos = ivec3(uniformVoxelRes * geomPass.voxelPos);

  for (int axis = 0; axis < 6; axis++)
    imageAtomicRGBA8Avg(voxelMaps[axis], voxelPos, vec4(radiance, 1));

    //imageAtomicCompSwap(voxelMaps[axis], voxelPos, 0, convVec4ToRGBA8(255*vec4((axis / 6.0) * geomPass.voxelPos.z, 0, 0, 1)));
    //imageAtomicCompSwap(voxelMaps[axis], voxelPos, 0, convVec4ToRGBA8(255 * vec4((axis/6.0)*radiance, 1)));

  /*for (int axis = 0; axis < 3; axis++) {
    vec3 voxelValue = float(geomPass.normal[axis] > 0.0) * radiance;
    imageAtomicRGBA8Avg(voxelMaps[axis * 2], voxelPos, vec4(voxelValue, 1));
    voxelValue = float(geomPass.normal[axis] < 0.0) * radiance;
    imageAtomicRGBA8Avg(voxelMaps[(axis * 2) + 1], voxelPos, vec4(voxelValue, 1));}*/
}
